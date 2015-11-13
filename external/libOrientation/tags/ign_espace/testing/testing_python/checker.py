# Pour la gestion des paths
import os
# Pour l'envoi de mails
import smtplib
# Information sur la plateforme
import sys
import platform
# Pour recuperer le nom de la machine
import socket
# Pour recuperer l'heure
from time import gmtime, strftime

class checker :
	def __init__(self, project = 'checker_python', recipients = ['didier.boldo@ign.fr', 'olivier.tournaire@ign.fr']) :
		self.project = project
		self.recipients = recipients

	def send_mail(self, corps_du_message) :
		print ("Sending mail...")
		FromWho = "From: " + self.project + ".codes-matis@ign.fr"
		# Ca, c'est le mail que l'on va envoyer ...
		message = FromWho + "\r\n"
		message += "To: "
		for recipient in self.recipients :
			message += recipient + ","
		message += "\r\nSubject: "+self.project+" compilation test\r\n\r\n\n"
		# Quelques infos de base
		message += "-----------------------------------------------\n"	
		message += "OS : " + os.name + "\n"
		if sys.platform != "win32":
			message += "Login : " + os.getlogin() + "\n"
		message += "Plateforme : " + platform.platform() + "\n"
		message += "Machine : " + socket.gethostname() + "\n"
		message += "Heure : " + strftime("%a, %d %b %Y %H:%M:%S +0000", gmtime()) + "\n"
		message += "-----------------------------------------------\n"
		message += corps_du_message
		print (FromWho)
		print (self.recipients)
		print (message)
		
		# On envoie le mail quelle que soit la plateforme ...
		SMTPserver = 'localhost'
		server = smtplib.SMTP(SMTPserver)
		server.sendmail(FromWho, self.recipients, message)
		server.quit()
		print ("Done ...")

	def exec_commande(self, commande, filename) :
		ERRORS = ['error','errors','erreur','erreurs']
		ERRORstr=[]
		for error in ERRORS :
			ERRORstr.append(error +' ')
			ERRORstr.append(error +':')
		commande += " > " + filename + " 2>&1"
		print (commande)
		os.system(commande)
		f = open(filename, 'r')
		output = f.read()
		print (output)
		message = ''
		output_lower = output.lower()
		for error in ERRORstr:
			if output_lower.count(error) > 0:
				message += "!!! Erreur pendant la commande :\n"
				message += commande + "\n"
				message += "-----------------------------------------------\n"
				message += output + "\n"
				message += "-----------------------------------------------\n"
				print (message)
				self.send_mail(message)
				return False
		print ("Done..")
		return True

	def compile_project(self) :
		print ("Compiling ...")
		commandeMAKE = ''
		if sys.platform == "win32":
			commandeMAKE = r'"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"'
			commandeMAKE+= ' && devenv '
			commandeMAKE+= self.project
			commandeMAKE+= '.sln /build Release /Project ALL_BUILD'
			commandeMAKE+= ' && devenv '
			commandeMAKE+= self.project
			commandeMAKE+= '.sln /build Debug  /Project ALL_BUILD'			
		else :
			commandeMAKE = 'make'
		return self.exec_commande(commandeMAKE, "out_compile.txt")
		  
	def test_project(self) :
		print ("Testing")
		commandeTEST = ''
		typePlateforme = sys.platform
		if typePlateforme == "win32":
			commandeTEST = r'"C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools\vsvars32.bat"'
			commandeTEST+= ' && devenv '
			commandeTEST+=self.project
			commandeTEST+='.sln /build Release /Project RUN_TESTS'
			commandeTEST+= ' && devenv '
			commandeTEST+=self.project
			commandeTEST+='.sln /build Debug  /Project RUN_TESTS'
		else :
			commandeTEST = 'make test ARGS="-V"'
		return self.exec_commande(commandeTEST, "out_test.txt")
		
	def check(self):
		print ("Debut check")
		currentDirectory = os.chdir('../..')
		if (self.exec_commande("svn update", "out_svn_update.txt") ) :
			currentDirectory = os.chdir('trunk/')
			if (self.exec_commande("cmake .", "out_cmake.txt") ) :
				if (self.compile_project()) :
					self.test_project()
		print ("Fin check")

if __name__ == "__main__" :
	mycheck = checker()
	mycheck.send_mail('Test interne du script python')
