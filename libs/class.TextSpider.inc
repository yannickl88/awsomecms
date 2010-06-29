<?php
class TextSpider
{
    private $table;

    private $id;
    private $words;

    public function __construct($table)
    {
        $this->table = $table;
        $this->words = array();
        $this->id = 1;
    }

    public function index($string, $link)
    {
        $word = "";
        $start = 0;
        $end = strlen($string) - 1;

        while($start < $end)
        {
            $letter = $string[$start];

            if(preg_match('/[:;!? ]/i', $letter) > 0)
            {
                if(!empty($word))
                {
                    $this->link(strtolower($word), $link);
                }

                $word = "";
            }
            else if(preg_match('/[a-z@\-_&\.]/i', $letter) > 0)
            {
                $word .= $letter;
            }

            $start++;
        }

        if(!empty($word))
        {
            $this->link(strtolower($word), $link);
        }

        var_dump($this->words);
    }

    private function link($word)
    {
        if(strlen($word) > 2)
        {
            if(!isset($this->words[$word]))
            {
                $this->words[$word] = 0;
            }
    
            $this->words[$word]++;
        }
    }

    public function search($words)
    {
        return array();
    }
}