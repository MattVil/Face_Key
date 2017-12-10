<?php
    include "include.php";

    $id = $_GET['id'];

    $ac = get_info("SharedAccount", $id, "id_account", "id_sharedAccount");

    $thisaccount = "account.php?id=$ac";
    $view = "Location: $thisaccount";
    $thisedit = "./eshac.php?id=$id";
    $thisdelete = "./dshac.php?id=$id";
    $exist = false;

    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;

            if($k=="pseudo"){
                $pr = $_POST[$k];
                $new_id_user = get_info("Users", $_POST[$k], "id_user", "pseudo");
                edit_table("SharedAccount",$id,"id_receiver", $new_id_user, "id_sharedAccount");  
 
            }
            else{
                edit_table("SharedAccount",$id,$k,$_POST[$k],"id_sharedAccount");   
            }
        }
    }

    if($exist){
        header($view);
    }

    $id_account = get_info("SharedAccount", $id, "id_account", "id_sharedAccount");
    $id_user = get_info("SharedAccount", $id, "id_receiver", "id_sharedAccount");
    $pseudo = get_info("Users", $id_user, "pseudo", "id_user") ;
    $expiration_date = get_info("SharedAccount", $id, "expiration_date", "id_sharedAccount");
    
    $acc = "./account.php?id=$ac";

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
                    <a href="<?php echo $acc?> " class="gradient-2">Back to con</a>
                    <a href="<?php echo $thisdelete; ?>" class="gradient-2">Delete</a>
                </div>
            </div>
            <h2 class="gradient-1">Edit of <?php echo get_info("Sites",$id_account,"domain","id_site")?> shared with <?php echo $pseudo?></h2>
            <form action="<?php $thisedit?>" method="post">
                <ul>
                    <li> <strong>Shared with :</strong> <select name="pseudo"><?php echo get_list("Users","pseudo",$pseudo)?></select></li>
                    <li> <strong>Until :</strong> <input name="expiration_date" placeholder="<?php echo $expiration_date ?>"/></li>
                </ul>
                <div class="center">
                    <input type="submit" value="Submit">
                </div>
            </form>
        </div>
    </body>
</html>
