import os

def post_bucket():
	"""
	Autor: Daniel Correa

	Permite crear un bucket para envio de datos en el servidor initialstate
	"""
	#Ejecucion de comando encargado de crear el bucket arduino bucket
	os.system('curl --include \
		--request POST \
		--header "Content-Type: application/json" \
		--header "X-IS-AccessKey: g7ewgXNOeJpaizQaCLJ7kwtswBNKmUl9" \
		--header "Accept-Version: 0.0.1" \
		--data-binary "{ \"bucketKey\": \"arduino_steam_bucket\", \"bucketName\": \"Arduino Bucket\"}" \
		"https://groker.initialstate.com/api/buckets"')

post_bucket()
