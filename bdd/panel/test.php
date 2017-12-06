<?php
	//including the database connection file
	//include 'local.postgre.conf.php';
	include 'postgresql.conf.inc.php'; 
	include 'fonction.php';
 

    add_to_table("users","name","Nv");
    add_to_table("users","first_name","NvFN");
    add_to_table("users","pseudo","nickname");
    
    //edit_table("users",5,"first_name","RIPIDEL");

    header("Location:userview.php");

?>    
