#!/usr/bin/python

import os
import sys
import math
import cgi
from urlparse import urlparse, parse_qs

#HTML and CGI init
print("Content-Type: text/html\n")
form = cgi.FieldStorage()


#Returns true if the user is logged in
def userLoggedIn(name):
	if name == None:
		return False
	f = open ('source/loggedin.csv')
	for line in f.readlines():
		if str(line[:-2]) == name:
			f.close()
			return True
	f.close()
	return False

#Checking if the user is logged in
username = form.getfirst("username")
userLoggedIn = userLoggedIn(username)

#Parsing the query string
quantities = form.getlist("quantity")
query = parse_qs(os.environ['QUERY_STRING'])
items = []
for k,v in query.items():
	if k[:4] == "item":
		items.append((int(k[5:-1]), str(v)[2:-2]))

#Reading Inventory.csv
f = open('Inventory.csv', 'r')
lines = f.readlines()
f.close()
inv = {}
for line in lines:
	l = line[:-1].split(',')
	inv.update({l[0]:(int(l[1]),float(l[2]), l[3])})

#Returns the tuple(qty, price) associated with a name
def getInfo(name):
	for k,v in inv.items():
		if k == name:
			return v
	return None

#Format a number to have two decimal numbers and a $
def moneyFormat(n):
	n /= 1.0
	n = round(n, 2)
	integer = int(math.floor(n))
	n -= integer
	s = str(n)[1:]
	if len(s) < 3:
		s += "0"
	return "$" + str(integer) + s 
print ("""<style> 
		body {	
			color:5C604D;
			background-image: url("images/background.png");
			background-repeat:no-repeat;
			background-attachment:fixed;
		}

		table { background-color: F20056;border: 1px dashed white;}
		td { padding: 10px 20px; text-align: center;}
		tr { background-color: pink;}
		tr.dark { background-color: F20056; color: white;}
		
		a:link {
			color: white;
			text-decoration: none;
		}
		
		a:hover {
			color: pink;
		}

		a:visited {
			color: white;
			text-decoration: none;
		}
		
		.text {
			border: 1px dashed white;
			margin: 100px;
			background-color: pink;
			opacity: 0.85;
		} 
	</style>
""")
print ("<body><center>")
if userLoggedIn:
	print ("<table>")
	print ("<tr class='dark'><td>Item</td> <td>Quantity</td>")
	print ("<td>Price/unit</td> <td>Cost</td></tr>")
	total = 0
	problems = []

	for i in range(len(items)):
		qty = int(float(quantities[items[i][0]]))
		name = items[i][1]	
		if qty > 0:
			info = getInfo(name)
			available = int(info[0])
			price = float(info[1])
			fullName = info[2]
			if  qty > available:
				problems.append((name, qty, fullName, available))
			if available > 0:
				realQty = min(qty, available)
				print("<tr>")
				print("<td>" + fullName + "</td><td>" + str(realQty) + "</td>")
				print("<td>" + moneyFormat(price) + "</td>")
				cost = price * realQty
				total += cost
				print("<td>" + moneyFormat(cost) + "</td>")
				print("</tr>")
	
				#updating inventory
				f = open('Inventory.csv', 'r')
				lines = f.readlines()
				f.close()
				f = open('Inventory.csv', 'w')	
				for line in lines:
					split = line.split(',')
					if split[0] == name:
						newline = name + ','
						newline += str(available - realQty) + ','
						newline += str(price) + "," + fullName + "\n"
						f.write(newline)
					else:
						f.write(line)	
				f.close()
	print("<tr class='dark'><td>TOTAL</td><td></td><td></td>")
	print("<td>"+ moneyFormat(total) + "</td></tr>")
	print ("</table></center>")
	
print("<div class='text'>")
if userLoggedIn and len(problems) > 0:
	print("The following items were not available ")
	print("in the specified quantities:<br><ul>")
	for p in problems:
		print("<li>" + p[2] + "  (" + str(p[1]) + " items requested")
		print(", " + str(p[3]) + " available)</li>")
	print("</ul>")
	print("<br><br>")
if not userLoggedIn:
	print("<h1>Please log in to make a purchase!</h1>")
	print("<br><br>Visit the <a href='login.html'>login page</a>.")
print("<br>Browse the <a href='catalogue.html'>catalogue</a>.")
print("<br>Return to the <a href='home.html'>home page</a>.")
print("</div></body>")

