<?php
    include "include.php";


$id = $_GET['id'];
$result = pg_query( "DELETE FROM SharedAccount WHERE id_sharedAccount=$id");
header("Location:userview.php");

?>
