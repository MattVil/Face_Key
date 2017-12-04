<?php
function createGraph($user){
		$ind = decrypt_ind($ind);

		$path = "./users/$user/freq.csv";

		$day = array();
		$nb_conn = array();

		//lecture CSV
		if (($handle = fopen($path, "r")) !== FALSE) {
			while (($data = fgetcsv($handle, 10000, ";")) !== FALSE) {
				array_push($nb_conn, $data[2]);
				array_push($day, $data[1]);
			}
			fclose($handle);
		}
		fclose($handle);

	  $width = 600;
		$height = 300;

	  $graph = new Graph($width,$height,'auto');
	  $graph->SetScale("intlin");

		$graph->SetShadow();
		$graph->SetMargin(40,30,20,40);

		$b1plot = new BarPlot($nb_conn);
		$bplot->SetFillColor('orange');
		$graph->Add($b1plot);


		$b1plot->SetColor("white");

		$graph->title->Set('Number of connection per day until the creations');
		$graph->xaxis->title->Set('Day number');
		$graph->yaxis->title->Set('Number of connections');

		$graph->title->SetFont(FF_FONT1,FS_BOLD);
		$graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);
		$graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);

		$graph->Stroke();
	}
?>
