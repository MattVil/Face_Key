<?php
	define('HOST','localhost');
	define('USER','web10509_lhar');
	define('PASS','azertyuiop');
	define('DB','web10509_db');
	
	$conn = new mysqli(HOST,USER,PASS,DB) ;

	if ($conn->connect_error) {
    	die("Connection failed: " . $conn->connect_error);
	} 

		
	$image = $_POST['image'];
			
	$sql = "INSERT INTO photo (image) VALUES ('$image')";

	if ($conn->query($sql) === TRUE) {
	    echo "New image successfully inserted";
	} else {
	    echo "Error: " . $sql . "<br>" . $conn->error;
	}

	$conn->close();


?>
