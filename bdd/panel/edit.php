<?php
    include 'local.postgre.conf.php';
    //include 'postgresql.conf.inc.php'; 
    include 'fonction.php';

    $id = $_GET['id']; 
    
    $view = "Location: userview.php";
    $thisedit = "/edit.php?id=$id";
    $profile = "/profile.php?id=$id";

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            edit_table("users",$id,$k,$_POST[$k]);
        }
    }
    if($exist){
        header($view);
    }
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
    <a href="userview.php">Back</a><br/>
    <a href="<?php echo $profile ?>">Edit</a>
    <br/>
    <br/>

    <form action="<?php $thisedit ?>" method="post">

        <ul>
            <li> name : <input type="text" name="name" placeholder="<?php echo get_info("users", $id, "name") ?>"/></li>
            <li> first Name : <input type="text" name="first_name" placeholder="<?php echo get_info("users", $id, "first_name") ?>"/></li>
            <li> pseudo : <input type="text" name="pseudo" placeholder="<?php echo get_info("users", $id, "pseudo") ?>"/></li>
            <li> gender : <input type="text" name="gender" placeholder="<?php echo get_info("users", $id, "gender") ?>"/></li>
            <li> mail : <input type="text" name="mail" placeholder="<?php echo get_info("users", $id, "mail") ?>"/></li>
            <li> Face Key password : <input type="text" name="password" placeholder="<?php echo get_info("users", $id, "password") ?>"/></li>
            <li> creation date : <input type="text" name="creation_date" placeholder="<?php echo get_info("users", $id, "creation_date") ?>"/></li>
            <li> language : <input type="text" name="language" placeholder="<?php echo get_info("users", $id, "language") ?>"/> </li>
        </ul> 
        <input type="submit" value="Submit">
    </form>
    
    </body>
</html>