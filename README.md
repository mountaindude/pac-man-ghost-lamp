# pac-man-ghost-lamp
MQTT over wifi (using ESP8266) controlled night lamp with RGB light effects.  
  
In the default setup, the lamp blinks green when a Qlik Sense Enterprise connection is opened or a session is started, and red when a connection is closed or a session is stopped.

Originally intended to be used as user activity monitor for Qlik Sense enterprise environments, but given that the lamp is controlled by MQTT messages, it can be used in any MQTT setting.

Copy the src/secrets_template.h file to a file named src/secrets.h, then edit the new file as needed wrt  
* the wifi network the lamp will be used in, and
* the IP address of the MQTT broker to be used  

More extensive blog post over at [Ptarmigan Labs](https://ptarmiganlabs.com/blog/2017/05/01/let-there-be-blinky-light/).



<a href="http://www.youtube.com/watch?feature=player_embedded&v=T_IxQYdoqJA" target="_blank"><img src="http://img.youtube.com/vi/T_IxQYdoqJA/0.jpg" 
alt="Qlik Sense activity monitor" width="240" height="180" border="10" /></a>

