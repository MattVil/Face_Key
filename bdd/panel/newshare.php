<?php
    include "include.php"

    $id = $_GET['id'];


    $thisaccount = "account.php?id=$id";
    $view = "Location: $thisaccount";
    $exist = false;

    $champ = " ";
    $value = " ";
    $_POST = array_filter($_POST);
    foreach($_POST as $k => $v){
        if(isset($_POST[$k])){
            $exist = true;
            if($k=="pseudo"){
                $pr = $_POST[$k];
                $new_id_user = get_info("Users", $_POST[$k], "id_user", "pseudo");
                $champ = insert_request_champ("id_receiver",$champ);
                $value = insert_request_value($new_id_user, $value);
 
            }
            else{
                $champ = insert_request_champ($k,$champ);
                $value = insert_request_value($_POST[$k], $value);
            }
            /*echo "ch : " . $champ . "<br>";
            echo "val : " . $value . "<br>";
            echo "<br>";*/
            // echo "champ : $champ || value $value <br/>";
        }
    }
    if($exist){
        //echo "<br><br><br><br>final : <br>champs = " . $champ ."<br>value = " . $value . "<br>";
        add_to_table("SharedAccount",$champ,$value);
        header($view);
    }


    $id_site = get_info("account", $id, "id_site", "id_account");
    $domain = get_info("Sites", $id_site, "domain", "id_site");
    $id_user = get_info("account", $id, "id_user", "id_account");
    $pseudo = get_info("Users", $id_user, "pseudo", "id_user") ;
    

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
    <div class="center">
        <a href="userview.php">Back</a>
        <a href="<?php echo $acc?> ">Back to con</a>
    </div>
    <h1>Share <?php echo $domain?> of <?php echo $pseudo?></h1>
    <form action="<?php $thisedit?>" method="post">
        <ul>
            <li> Shared with : <select name="pseudo"><?php echo get_list("Users","pseudo",0)?></select></li>
            <li> Until : <input name="expiration_date" placeholder="yyyy-mm-dd"/></li>
        </ul>
        <div class="center">
            <input type="submit" value="Submit">
        </div>
    </form>
    </body>
</html>
