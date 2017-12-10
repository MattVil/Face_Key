<?php
    include "include.php";


$id = $_GET['id'];
$result = pg_query( "DELETE FROM PaymentAccount WHERE id_account=$id");
header("Location:userview.php");

?>
