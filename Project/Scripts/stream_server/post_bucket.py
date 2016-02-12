import os

def post_bucket():
	os.system('curl --include \
		--request POST \
		--header "Content-Type: application/json" \
		--header "X-IS-AccessKey: g7ewgXNOeJpaizQaCLJ7kwtswBNKmUl9" \
		--header "Accept-Version: 0.0.1" \
		--data-binary "{ \"bucketKey\": \"arduino_steam_bucket\", \"bucketName\": \"Arduino Bucket\"}" \
		"https://groker.initialstate.com/api/buckets"')

post_bucket()