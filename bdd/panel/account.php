<?php
    //include 'local.postgre.conf.php';
    include 'postgresql.conf.inc.php';
    include 'fonction.php';

    $id = $_GET['id'];

    $view = "Location: userview.php";
    $thisedit = "/account.php?id=$id";
    $profile = "/profile.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            edit_table("account",$id,$k,$_POST[$k]);
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
    <a href="profile.php?id= <?php echo real_get_info("account", $id, "id_user", "id_account");?> ">Back</a><br />
    <a href="<?php echo $profile; ?>">Edit</a>
    <br/>
    <br/>

    <form action="<?php $thisedit ?>" method="post">

        <ul>
            <li> site : <input type="text" name="site" placeholder="<?php echo real_get_info("account", $id, "id_site", "id_account") ;?>"/></li>
            <li> login : <input type="text" name="login" placeholder="<?php echo real_get_info("account", $id, "login", "id_account") ;?>"/></li>
            <li> password : <input type="text" name="password" placeholder="<?php echo real_get_info("account", $id, "password", "id_account") ;?>"/></li>
            <li> tag : <input type="text" name="tag" placeholder="<?php echo real_get_info("account", $id, "id_tag", "id_account") ;?>"/></li>
        </ul>
        <input type="submit" value="Submit">
    </form>

    </body>
</html>
