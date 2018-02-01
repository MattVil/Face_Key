<?php
    include "include.php";


$id = $_GET['id'];

$result = pg_query( "DELETE FROM account WHERE id_site=$id NOCHECK CONSTRAINT dataaccount_id_account_fkey");
$result = pg_query( "DELETE FROM Sites WHERE id_site=$id NOCHECK CONSTRAINT account_id_site_fkey");

header("Location:siteview.php");

?>
