<?php
  include 'local.postgre.conf.php';
  //include 'postgresql.conf.inc.php'; 
  include 'fonction.php';


$id = $_GET['id']; 
$result = pg_query( "DELETE FROM users WHERE id_user=$id");
header("Location:userview.php");

?>
