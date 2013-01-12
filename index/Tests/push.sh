#!/bin/bash

endPoint=http://ec2-50-17-15-66.compute-1.amazonaws.com:11111/Dorafle.cgi/push
data=../../Documentation/crawler-json.txt

curl --data @$data $endPoint
