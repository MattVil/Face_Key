<?php
  include 'local.postgre.conf.php';
  //include 'postgresql.conf.inc.php'; 
  include 'fonction.php';


$id = $_GET['id']; 
?>

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        
        <link rel="stylesheet" href="css/main.css">
        <link rel="shortcut icon" type="image/png" href="img/favicon.png">
        
        <title>Facekey &mdash; Admin Panel</title>
    </head>
    <body>
    <a href="userview.php">Back</a><br/><br/>

	<ul>
		<li> name : <?php echo get_info("users", $id, "name") ?></li>
		<li> first Name : <?php echo get_info("users", $id, "first_name") ?></li>
		<li> pseudo : <?php echo get_info("users", $id, "pseudo") ?></li>
		<li> gender : <?php echo get_info("users", $id, "gender") ?></li>
		<li> mail : <?php echo get_info("users", $id, "mail") ?></li>
		<li> password : <?php echo get_info("users", $id, "password") ?></li>
		<li> creation date : <?php echo get_info("users", $id, "creation_date") ?></li>
		<li> language : <?php echo get_info("users", $id, "language") ?></li>

	</ul> 

	<ul>

	</ul>
    
    </body>
</html>