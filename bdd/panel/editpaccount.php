<?php
    include "include.php";

    $id = $_GET['id'];

    $thisaccount = "account.php?id=$id";
    $view = "Location: $thisaccount";
    $thisedit = "./editpacaccount.php?id=$id";
    $thisdelete = "./dpaccount.php?id=$id";
    $check = "./paccount.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;

            if($k=="domain"){
                $pr = $_POST[$k];
                $new_id_site = get_info("Sites", $_POST[$k], "id_site", "domain");
                edit_table("account",$id,"id_site", $new_id_site, "id_account");  
 
            }
            else if($k=="tag"){
                $new_id_tag = get_info("Tags", $_POST[$k], "id_tag", "name_tag");
                edit_table("account",$id,"id_tag",$new_id_tag,"id_account");   

            }

            else{
                edit_table("account",$id,$k,$_POST[$k],"id_account");   
            }
        }
    }

    if($exist){
        header($view);
    }

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
            <div class="row vcenter">
                <div class="col-6 ">
                  <h1 class="gradient-4"><a href="./index.php" style="font-weight: 900;">Facekey Admin Panel</a></h1>
                </div>
                <div class="col-6 right">
                    <a href="userview.php" class="gradient-2">Back</a>
                    <a href="<?php echo $profile?> " class="gradient-2">Profile</a>
                    <a href="<?php echo $check; ?>" class="gradient-2">Check</a>
                    <a href="<?php echo $thisdelete; ?>" class="gradient-2">Delete</a>

                </div>
            </div>
            <h2 class="gradient-1">Edit paying account of <?php echo $pseudo;?></h2>
            <form action="<?php $thisedit?>" method="post">
                <ul>
                    <li> <strong>Site :</strong> <select name="domain"><?php echo get_list("Sites","domain",$domain)?></select></li>
                    <li> <strong>Login :</strong> <input name="login" placeholder="<?php echo $login?>"/></li>
                    <li> <strong>Password :</strong> <input name="password" placeholder="<?php echo $password ?>"/></li>
                    <li> <strong>Bank : <input name="bank" placeholder="<?php echo $bank ?>"/></li>
                    <li> <strong>RIB :</strong> <input name="rib" placeholder="<?php echo $rib ?>"/></li>
                    <li> <strong>Card Number :</strong> <input name="card_num" placeholder="<?php echo $card_num ?>"/></li>
                    <li> <strong>Cryptogram :</strong> <input name="cryptogram" placeholder="<?php echo $cryptogram ?>"/></li>
                    <li> <strong>Tag :</strong> <select name="tag"><?php echo get_list("Tags","name_tag",$tag)?></select></li>
                </ul>
                <div class="center">
                    <input type="submit" value="Submit">
                </div>
            </form>
        </strong>
    </body>
</html>
