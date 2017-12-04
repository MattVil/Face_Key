<?php/*
    if (isset($_GET["dbname"])){
    }else{
    echo "<script>javascript:history.go(-1)</script>";
    }
    include 'local.postgre.conf.php';
    //include 'postgresql.conf.inc.php'; 
    include 'fonction.php';*/
?>
<?php
  include 'local.postgre.conf.php';
  //include 'postgresql.conf.inc.php'; 
  include 'fonction.php';
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
    <a href="add.php">Add New Data</a><br/><br/>
    <a href="test.php">Test</a><br/><br/>
 
    <?php echo display_table("users"); ?>
    </body>
</html>