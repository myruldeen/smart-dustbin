# smart-dustbin
## Here is the list of commands that is required for successful installation of mosquitto and mosquitto clients on the raspbian buster OS.
sudo apt-get update  
wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key  
sudo apt-key add mosquitto-repo.gpg.key  
cd /etc/apt/sources.list.d/  
sudo wget http://repo.mosquitto.org/debian/mosquitto-stretch.list  
sudo apt-get update  
cd ~  
sudo apt-get update  
sudo apt-get install mosquitto mosquitto-clients  
sudo /etc/init.d/mosquitto stop  
sudo nano /etc/mosquitto/mosquitto.conf  

## Delete these lines in the mosquitto.conf file
log_dest file /var/log/mosquito/mosquito.log  
include_dir /etc/mosquito/conf.d

## Add the lines below instead
log_dest topic  
log_type error  
log_type warning  
log_type notice  
log_type information  
connection_messages true  
log_timestamp true  
include_dir /etc/mosquitto/conf.d  

## After adding the lines, now press CTRL+X and press Y and then enter to save the file. Now start the mosquitto broker.
sudo /etc/init.d/mosquitto start
