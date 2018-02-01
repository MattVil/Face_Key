<?php
    include "include.php";


$id = $_GET['id'];
$result = pg_query( "DELETE FROM account WHERE id_user=$id");
$result = pg_query( "DELETE FROM users WHERE id_user=$id");
header("Location:userview.php");

?>
