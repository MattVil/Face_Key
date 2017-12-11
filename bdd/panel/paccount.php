<?php
    include "include.php";

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
    $pseudo = get_info("users", $id_user, "pseudo", "id_user");
    
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
        <div class="container">
            <div class="row vcenter nav-back">
            </div>
            <div class="row nav">
                <div class="container">
                    <div class="row vcenter">
                        <div class="col-6 ">
                            <div class="row">
                                <h1 class="left"><a class="logo" href="./index.php">Facekey Admin Panel</a></h1>
                            </div>
                        </div>
                        <div class="col-6 right">
                            <div class="row vcenter">
                                <div class="">
                                </div>
                                <div class="col-8">
                                    <a href="<?php echo $profile ?>" class="gradient-22 nava">Profile</a>
                                    <a href="<?php echo $thisedit ?>" class="gradient-22 nava">Edit</a>
                                    <a href="<?php echo $thisdelete ?>" class="gradient-22 nava">Delete</a>
                                </div>
                                <div class="col-4">
                                    <a href="./userview.php" class="nava white">Users </a>
                                    <a href="./siteview.php" class="nava white">Sites </a>

                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="nav-post">
            </div>
            <h2 class="gradient-8">Paying account of <?php echo $pseudo;?></h2>
            <form action="<?php $thisedit?>" method="post">
                <ul>
                    <li> <strong>Site :</strong> <?php echo $domain?></li>
                    <li> <strong>Login :</strong> <?php echo $login?></li>
                    <li> <strong>Password :</strong> <?php echo $password ?></li>
                    <li> <strong>Bank :</strong> <?php echo $bank ?></li>
                    <li> <strong>RIB :</strong> <?php echo $rib ?></li>
                    <li> <strong>Card Number :</strong> <?php echo $card_num ?></li>
                    <li> <strong>Cryptogram :</strong> <?php echo $cryptogram ?></li>
                    <li> <strong>Tag :</strong> <?php echo $tag?></li>
                </ul>
                <div class="center">
                    <input type="submit" value="Submit">
                </div>
            </form>
        </div>
    </body>
</html>
