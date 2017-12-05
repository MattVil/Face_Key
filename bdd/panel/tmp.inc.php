<?php
function createGraph($user){

	$path = "./users/$user/freq.csv";
	$day = array();
	$nb_conn = array();

	//lecture CSV
	if (($handle = fopen($path, "r")) !== FALSE) {
			while (($data = fgetcsv($handle, 10000, ";")) !== FALSE) {
					array_push($nb_conn, $data[1]);
					array_push($day, $data[0]);
					// echo "nb con : " . $data[0] . " | day : " . $data[1] . "<br/>";
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
	$b1plot->SetFillColor('orange');

	$graph->Add($b1plot);



	$b1plot->SetColor("white");

	$graph->title->Set('Number of connection per day until the creations');
	$graph->xaxis->title->Set('Day number');
	$graph->yaxis->title->Set('Number of connections');

	$graph->title->SetFont(FF_FONT1,FS_BOLD);
	$graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);
	$graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);


	$gdImgHandler = $graph->Stroke(_IMG_HANDLER);
	$fileName = "./users/$user/graphFreq.png";

	$graph->img->Stream($fileName);
	
	}
?>
