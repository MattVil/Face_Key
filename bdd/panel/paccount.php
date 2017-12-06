<?php
    //include 'local.postgre.conf.php';
    include 'postgresql.conf.inc.php';
    include 'fonction.php';

    $id = $_GET['id'];

    $view = "Location: userview.php";
    $thisedit = "/paccount.php?id=$id";
    $profile = "/profile.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            edit_table("paymentaccount",$id,$k,$_POST[$k]);
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
    <a href="profile.php?id= <?php echo get_info("paymentaccount", $id, "id_user", "id_account");?> ">Back</a><br />
    <a href="<?php echo $profile; ?>">Edit</a>
    <br/>
    <br/>

    <form action="<?php $thisedit ?>" method="post">

        <ul>
            <li> site : <input type="text" name="site" placeholder="<?php echo get_info("paymentaccount", $id, "id_site", "id_account") ;?>"/></li>
            <li> login : <input type="text" name="login" placeholder="<?php echo get_info("paymentaccount", $id, "login", "id_account") ;?>"/></li>
            <li> password : <input type="text" name="password" placeholder="<?php echo get_info("paymentaccount", $id, "password", "id_account") ;?>"/></li>
            <li> tag : <input type="text" name="tag" placeholder="<?php echo get_info("paymentaccount", $id, "id_tag", "id_account") ;?>"/></li>
            <li> bank : <input type="text" name="tag" placeholder="<?php echo get_info("paymentaccount", $id, "bank", "id_account") ;?>"/></li>
            <li> rib : <input type="text" name="tag" placeholder="<?php echo get_info("paymentaccount", $id, "rib", "id_account") ;?>"/></li>
            <li> card number : <input type="text" name="tag" placeholder="<?php echo get_info("paymentaccount", $id, "card_num", "id_account") ;?>"/></li>
            <li> cryptogram : <input type="text" name="tag" placeholder="<?php echo get_info("paymentaccount", $id, "cryptogram", "id_account") ;?>"/></li>
        </ul>
        <input type="submit" value="Submit">
    </form>

    </body>
</html>
