#move_logs.py
import os,shutil
import datetime

def move_logs():
	"""
	Autor: Daniel Correa

	Permite mover los logs al directorio old_logs
	"""
	date = datetime.date.today() #Obtenemos la fecha actual
	shutil.copy('sensor.log','old_logs/sensor_log_until_'+str(date)+'.log') #Copiamos el archivo log al directorio old_logs
	os.remove('sensor.log') #Eliminamos el archivo sesnor.logs redudante
	print('Archivo log enviado a old_logs, nuevos logs a partir de -> '+str(date)) #Informamos la accion

move_logs()
