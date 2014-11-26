#!/usr/bin/python

import os
import math
import cgi
from urlparse import urlparse, parse_qs

#HTML and CGI init
print("Content-Type: text/html\n")
form = cgi.FieldStorage()

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
	inv.update({l[0]:(int(l[1]),float(l[2]))})

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
	return str(integer) + s + "$" 

print ("<style> td{padding: 10px;} </style>")
print ("<table>")
print ("<tr><td>Item</td> <td>Quantity</td>")
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
		if  qty > available:
			problems.append((name, qty))
		if available > 0:
			realQty = min(qty, available)
			print("<tr>")
			print("<td>" + name + "</td><td>" + str(realQty) + "</td>")
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
					newline += str(price) + "\n"
					f.write(newline)
				else:
					f.write(line)	
			f.close()
print("<tr><td>TOTAL</td><td></td><td></td>")
print("<td>"+ moneyFormat(total) + "</td></tr>")
print ("</table>")
if len(problems) > 0:
	print("<br><br><br>The following items were not available ")
	print("in the specified quantities:<br><ul>")
	for p in problems:
		print("<li>" + p[0] + "  (" + str(p[1]) + " items requested)</li>")
	print("</ul>")
print("<br><br><br>")
print("Go back to the <a href='catalogue.html'>catalogue page</a>.")
