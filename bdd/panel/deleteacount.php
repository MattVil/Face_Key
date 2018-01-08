<?php
    include "include.php";


$id = $_GET['id'];
$result = pg_query( "DELETE FROM dataaccount WHERE id_account=$id");
$result = pg_query( "DELETE FROM sharedaccount WHERE id_account=$id");
$result = pg_query( "DELETE FROM sharedaccount WHERE id_receiver=$id");
$result = pg_query( "DELETE FROM account WHERE id_account=$id");
header("Location:userview.php");

?>
