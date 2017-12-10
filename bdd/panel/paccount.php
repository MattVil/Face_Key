<?php
    include "include.php"

    $id = $_GET['id'];

    $thisaccount = "account.php?id=$id";
    $view = "Location: $thisaccount";
    $thisedit = "./editpaccount.php?id=$id";
    $thisdelete = "./dpaccount.php?id=$id";
    $check = "./paccount.php?id=$id";

   
    $id_site = get_info("paymentaccount", $id, "id_site", "id_account");
    $domain = get_info("Sites", $id_site, "domain", "id_site");
    $login = get_info("paymentaccount", $id, "login", "id_account");
    $password = get_info("paymentaccount", $id, "password", "id_account") ;
    $bank = get_info("paymentaccount", $id, "bank", "id_account") ;
    $rib = get_info("paymentaccount", $id, "rib", "id_account") ;
    $card_num = get_info("paymentaccount", $id, "card_num", "id_account") ;
    $cryptogram = get_info("paymentaccount", $id, "cryptogram", "id_account") ;
    $id_tag = get_info("paymentaccount", $id, "id_tag", "id_account");
    $tag = get_info("Tags", $id_tag, "name_tag", "id_tag");
    $id_user = get_info("paymentaccount", $id, "id_user", "id_account");
    
    $profile = "./profile.php?id=$id_user";

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
    <a href="userview.php">Back</a><br />
    <a href="<?php echo $profile?> ">Back to User Profile</a><br />
    <a href="<?php echo $thisedit; ?>">Check</a><br/>
    <a href="<?php echo $thisdelete; ?>">Delete</a>
    <br/>
    <br/>
    <form action="<?php $thisedit?>" method="post">
        <ul>
            <li> Site : <?php echo $domain?></li>
            <li> Login : <?php echo $login?></li>
            <li> Password : <?php echo $password ?></li>
            <li> Bank : <?php echo $bank ?></li>
            <li> Tib : <?php echo $rib ?></li>
            <li> Card Number : <?php echo $card_num ?></li>
            <li> Cryptogram : <?php echo $cryptogram ?></li>
            <li> Tag : <?php echo $tag?></li>
        </ul>
        <input type="submit" value="Submit">
    </form>
    </body>
</html>
