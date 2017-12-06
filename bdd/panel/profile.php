<?php
    include 'local.postgre.conf.php';
    // include 'postgresql.conf.inc.php';
    include 'fonction.php';

    include ('lib/jpgraph/src/jpgraph.php');
    include ('lib/jpgraph/src/jpgraph_bar.php');


    $id = $_GET['id'];
    $edit = "./edit.php?id=$id";
    $imgpath = "./users/$id/graphFreq.png";
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
    <a href="userview.php">Back</a>
    <br/>
    <a href="<?php echo $edit ?>">Edit</a>
	<br/><br/>

	<ul>
		<li> name : <?php echo get_info("users", $id, "name") ?></li>
		<li> first Name : <?php echo get_info("users", $id, "first_name") ?></li>
		<li> pseudo : <?php echo get_info("users", $id, "pseudo") ?></li>
		<li> gender : <?php echo get_info("users", $id, "gender") ?></li>
		<li> mail : <?php echo get_info("users", $id, "mail") ?></li>
		<li> Face Key password : <?php echo get_info("users", $id, "password") ?></li>
		<li> creation date : <?php echo get_info("users", $id, "creation_date") ?></li>
		<li> language : <?php echo get_info("users", $id, "language") ?></li>
  </ul>


		<h2>CO Proprietaire</h2>
		<?php echo display_table_query("SELECT account.id_account, domain, account.login, account.password FROM Account INNER JOIN Sites ON account.id_site = sites.id_site
      LEFT JOIN PaymentAccount ON account.id_account = paymentaccount.id_account
    WHERE account.id_user = $id AND paymentaccount.id_account IS NULL;", 1); ?>
    <?php echo display_table_query("SELECT id_account, domain, login, password, bank, rib, card_num, cryptogram FROM PaymentAccount INNER JOIN Sites ON paymentaccount.id_site = sites.id_site
    WHERE paymentaccount.id_user = $id;", 2); ?>

		<h2>CO partagée avec <?php echo get_info("users", $id, "pseudo") ?></h2>
		<?php echo display_table_query("SELECT id_sharedAccount, domain, name, first_name FROM SharedAccount INNER JOIN Account
      ON sharedAccount.id_account = account.id_account INNER JOIN Sites ON account.id_site = sites.id_site INNER JOIN Users ON account.id_user = users.id_user
      WHERE sharedAccount.id_receiver = $id"); ?>
    <h2>Graph</h2>
		<?php createGraph($id);?>
		<img src="<?php echo $imgpath ?>" alt="graphFreq"/>

  



    </body>
</html>
