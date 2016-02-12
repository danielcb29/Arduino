import os.path,datetime,shutil
import os

def modification_date(path):
	"""
	Autor: Daniel Correa
	Permite obtener la fecha de modificacion de un archivo dada su direccion path
	"""
	t = os.path.getmtime(path)
	return datetime.datetime.fromtimestamp(t)

def cron_arduino():
	"""
	Autor: Daniel Correa
	Permite verificar las fechas de modificacion de los archivos del Repositorio y de Produccion para actualizar el sketche en caso de exisistir modificaciones
	"""

	mod_archivo_repo = modification_date('/root/Proyecto/Repositorio/sketche.hex')
	mod_archivo_run = modification_date('/root/Proyecto/Produccion/sketche.hex')
	print('Fecha modificacion repositorio: ' + str(mod_archivo_repo))
	print('Fecha modificacion produccion: ' + str(mod_archivo_run))
	if mod_archivo_repo > mod_archivo_run: 
		print('Actualizacion de script principal en proceso...')
		#Traemos la version del repositorio
		shutil.copy('/root/Proyecto/Repositorio/sketche.hex','/root/Proyecto/Produccion/sketche.hex')
		#Llamamos funciones para cargar el .hex en la memoria del inino
		os.system("merge-sketch-with-bootloader.lua /root/Proyecto/Produccion/sketche.hex")
		os.system("run-avrdude /root/Proyecto/Produccion/sketche.hex")
	else: 
		print('No se ha realizado ninguna actualizacion')

cron_arduino()