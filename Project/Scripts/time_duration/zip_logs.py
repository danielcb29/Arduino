#zip_logs.py
import os
import zipfile
import datetime

def zipdir(path, ziph):
	"""
	Autor: Daniel Correa

	Permite ingresar los archivo del directorio ubicado en path al archivo .zip ziph, 
	elimina los archivos exisitentes en path despues de escribir dicho archivo en el .zip
	"""
    #Se obtiene el contenido del directorio y se comprime
    for root, dirs, files in os.walk(path):
        for file in files: #Por cada archivo en la lista de archivos
            ziph.write(os.path.join(root, file)) #Se escriben en el .zip
            os.remove(os.path.join(root, file)) #Se borran del directorio

def zip_logs():
	"""
	Autor: Daniel Correa

	Metodo principal, obtiene la fecha, crea el .zip y utiliza zipdir para escribirlos en el .zip
	"""
	date = datetime.date.today() #Se obtiene la fecha
	print("Ziping file in date -> " + str(date)) #Se informa la compresion
	zip_file = zipfile.ZipFile('zip_logs/logs_until_'+str(date)+'.zip', 'w') #Se crea el archivo .zip
	zipdir('old_logs',zip_file) #Se ingresan los logs al .zip

zip_logs()