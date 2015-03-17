<?php
function smarty_function_grid($params, &$smarty)
{
    $code = $params['code'];

    $grid = Table::init('imagegrid.imagegrid')
        ->setRequest((object) array("grid_name" => $code))
        ->doSelect()
        ->getRow();

    $blocks = Table::init("imagegrid.block")
        ->setRequest((object) array("block_grid" => $grid->grid_id))
        ->doSelect()
        ->getRows();

    $lang = getLang();
    usort($blocks, function($a, $b) use($lang) {
        return strcmp(strtolower($a->block_title[$lang]), strtolower($b->block_title[$lang]));
    });

    $number_of_blocks = count($blocks);
    $number_of_collumns = 2;

    if($grid->grid_columns == 0) {
        if($number_of_blocks <= 3) {
            $number_of_collumns = $number_of_blocks;
        } else if($number_of_blocks % 3 == 0) {
            $number_of_collumns = 3;
        } else if($number_of_blocks % 2 == 0) {
            $number_of_collumns = 2;
        } else {
            if($number_of_blocks % 2 <= $number_of_blocks % 3) {
                $number_of_collumns = 3;
            }
        }
    } else {
        $number_of_collumns = (int) $grid->grid_columns;
    }

    $number_of_rows = (int) ceil($number_of_blocks / $number_of_collumns);
    $blocks_in_last_row = $number_of_blocks % $number_of_collumns;

    if($blocks_in_last_row == 0) {
        $blocks_in_last_row = $number_of_collumns;
    }

    $html = "<table class='imagegrid'>\n";

    $index = 0;
    for($i = 0; $i < $number_of_rows; $i++) {
        $html .= "<tr class='row'>\n";
        for(
                $j = 0,
                $n = ($i == $number_of_rows - 1 && $grid->grid_fill) ? $blocks_in_last_row : $number_of_collumns;
                $j < $n && $index < $number_of_blocks;
                $j++
        ) {
            $width = floor(100 / $n);
            $image = Table::init("file.files")
                ->setRequest((object) array("file_id" => (int) $blocks[$index]->block_image[0]))
                ->doSelect()
                ->getRow();

            $html .= "<td style='width: {$width}%;' onclick='window.location=\"" . $blocks[$index]->block_url . "\"'>";
            $html .= "<h2>" . $blocks[$index]->block_title[$lang] . "</h2>\n";
            $html .= "<a href='" . $blocks[$index]->block_url . "'><img src='" . $image->file_data->url . "'>\n";
            $html .= "<div class=\"text\">" . BBCodeParser::parse($blocks[$index]->block_text[$lang], true) . "</div>";
            $html .= "</a>\n";
            $index++;
        }
        $html .= "</tr>\n";
    }
    $html .= "</table>\n";

    return $html;
}