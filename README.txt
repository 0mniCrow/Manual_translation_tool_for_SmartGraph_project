This application is a supporting translation tool for the main project "Smart Graph"
The application allows to add additional translations (or change existing ones) without the nessesity of changing source code of the main application.

The project uses XML files as an input resource and the resulting product. The input XML file contains a list of all translatable objects including 
widgets (they are marked as translatable with "_trsl" suffix in their object names by developers) and output messages (they are marked as QStrings
in "class_name" field). All these objects will be restructured from "XML DOM" model  to a table of objects in the main window of the application.
User can load an appropriate XML file by pressing "Load" button and choosing a .xml file in file dialog window. If the file has an appropriate structure,
the data from the file will be represented on the screen. 
The table consists of at least 3 main columns and 1 main row:
The first column named "Window" and following cells contain the object name of the parent window of the element.
The second column named "Name" and following cells contain the object name of the element.
The third column named "Type" and folloding cells contain the object class of the element. 
If the chosen file didn't contain any translations before, there will be only these three columns.
The first row of the table contain the names of languages. The first three cells of the row will be empty and non-editable.
Then user has these possible actions: 
1) Add a new translation with the button "Add". A new empty column will appear at the end of the table.
	1.1) After that user may start filling in the appeared column with translations. It is important to give a name to the translation in the first row.
2) Remove the last translation with the button "Remove". The last column will be removed.
3) If the loaded file contains translations, user can start editing it right after loading the project.
4) Save the translation project with the button "Save". The dialog window will appear and the user must choose the file for save.
5) Load another XML file with objects. All unsaved progress will be lost if not saved.

Only the cells in the translation columns can be edited. The first cell is responcible for the name of the language and will represent this language in
the list of available languages in the main application. 

The structure of the XML File:

<translation>								#root elements
--<window name="window_name">				#the first window. Attribute - object name of the window
--|--<object>								#element of the first window
--|--|--<name> object_name					#object name of the element
--|--|--<type> class_name					#class name of the element
--|--|--<translations>						#list of translations
--|--|--|--<language_name1>translation1		#the first translation. Tag name is the name of the language!
--|--|--|--<language_name2>translation2		#the second translation.
--|--|--|.....
--|--<object>								#next element of the first window
--|--|.....
--<window name="window_name2">				#next window
--|......
--<languages>								#languages list
--|--<language> language_name1				#the first language
--|--<language> language_name2				#the second language
--|.....
