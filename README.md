# spygroup


SpyGroup application runs in Asterisk and MySQL environment and used for spying on predefined groups of VoIP users. 
The spying operation is protected by unique password that used to select which group will be spyed on. 
All the data (group numbers, extensions list, passwords) are stored in MySQL database. 
The setspygroupd daemon checks all active calls for database extension match, 
if exact match found - the extension will be added to predefined group number. 
Spying can be done by dialing the special SpyGroup extension and entering the password.

Automatically exported from code.google.com/p/spygroup
