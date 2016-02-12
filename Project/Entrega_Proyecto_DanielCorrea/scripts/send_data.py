#stream_dc.py
import os
import json

def stream_data():
	"""
	Autor: Daniel Correa

	Permite enviar la informacion recuperada del log al servidor de initialstate
	"""
	#Lectura de cadena del archivo
	f = open('/root/Proyecto/Logs/sensor.log', 'r')
	line_one = f.readline().split(',')
	line_two = f.readline().split(',')
	#JSON que se enviara
	data = json.dumps([
		{'key':'date','value': line_one[0]},
		{'key': 'id','value': line_one[1]},
		{'key': 'temperature','value': line_one[3]},
		{'key': 'humidity','value': line_two[3]}
		])
	#Envio de datos a initialstate
	os.system("curl -k --include  --request POST  --header 'Content-Type: application/json'  --header 'X-IS-AccessKey: g7ewgXNOeJpaizQaCLJ7kwtswBNKmUl9'  --header 'X-IS-BucketKey: arduino_steam_bucket'  --header 'Accept-Version: 0.0.1'  --data-binary '"+data.__str__()+"'  'https://groker.initialstate.com/api/events'")
stream_data()
