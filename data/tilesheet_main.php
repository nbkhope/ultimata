<?php

$TILESIZE = 48;
$tiles_across = 16;
$tiles_down = 16;

$m = 0;
$n = 0;

for ($m = 0; $m < tiles_down; $m++) {
	for ($n = 0; $n < tiles_across; $n) {
		$filename = "tilesheet_main_-" . $m . "-" . $n . ".jpg";

		echo "<img src='tilesheet_main/$filename' alt='tile ($m, $n)' title='($m, $n)' width='$TILESIZE' height='$TILESIZE'>\n";
	}
	echo "<br>";
}



?>