<?php
    include "include.php"


$id = $_GET['id'];
$result = pg_query( "DELETE FROM Sites WHERE id_site=$id");
header("Location:siteview.php");

?>
