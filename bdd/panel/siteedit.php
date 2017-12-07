<?php
    // include 'local.postgre.conf.php';
    include 'postgresql.conf.inc.php';
    include 'fonction.php';

    $id = $_GET['id'];

    $view = "Location: siteview.php";
    $thisedit = "./siteedit.php?id=$id";
    $profile = "./sitedetail.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            edit_table("Sites",$id,$k,$_POST[$k],"id_site");
        }
    }
    if($exist){
        header($view);
    }

    $domain = get_info("Sites", $id, "domain", "id_site");
    $login_input = get_info("Sites", $id, "login_input", "id_site");
    $password_input = get_info("Sites", $id, "password_input", "id_site");

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
    <a href="<?php echo $profile ?>">Site</a>
    <br/>
    <br/>
    <h1>Edit of <?php echo $domain ?></h1>

    <form action="<?php $thisedit ?>" method="post">

         <ul>
            <li> Id : <input type="text" name="id" placeholder="<?php echo  $id?>"/></li>
            <li> Domain : <input type="text" name="domain" placeholder="<?php echo $domain?>"/></li>
            <li> Input for login : <input type="text" name="login_input" placeholder="<?php echo $login_input ?>"/></li>
            <li> Input for Passwor : <input type="text" name="password_input" placeholder="<?php echo $password_input?>"/></li>
        </ul>
        <input type="submit" value="Submit">
    </form>

    </body>
</html>
