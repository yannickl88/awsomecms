<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package libs
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Spider class for indexing and searching in the text items
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TextSpider
{
    private $id;
    private $id2words;
    private $words2id;
    private $newWords;
    /**
     * Constructor
     */
    public function __construct()
    {
        $this->id2words = array();
        $this->words2id = array();
        $this->newWords = array();
        $this->id = 0;
        
    }
    /**
     * Index a text block, the found words and matches are stored in the database
     * 
     * @param String $string
     * @param int $link
     * @param String $hook
     */
    public function index($string, $link, $title, $hook)
    {
        //fetch already existing words
        $results = SQLQuery::doSelect("word_id")
            ->select("word_word")
            ->table("search_words")
            ->exec();
        
        while($result = $results->getRow())
        {
            $this->id2words[$result->word_id] = $result->word_word;
            
            if($result->word_id > $this->id)
            {
                $this->id = (int) $result->word_id;
            }
        }
        
        $this->words2id = array_flip($this->id2words);
        
        //create a new item for this if not already there
        $check = md5($link);
        $item = -1;
        
        $result = SQLQuery::doSelect()
            ->table("search_items")
            ->where("item_checksum", $check)
            ->exec();
        
        if($result->count() > 0)
        {
            $item = $result->getRow()->item_id;
            $this->clearByID($item);
        }
        else
        {
            $item = SQLQuery::doInsert()
                ->table("search_items")
                ->insert("item_link", $link)
                ->insert("item_title", $title)
                ->insert("item_checksum", $check)
                ->insert("item_hook", $hook)
                ->exec()
                ->insertID();
        }
            
        $string = trim(preg_replace(array('/(\s+)/', '/(<.*?>)/', '/({.*?})/'), " ", str_replace(array(".", ",", "'", "\"", "\n", "\r", "“"), " ", strtolower($string))));
        $words = array();
        $offset = 0;
        $break = false;
        
        while(!$break)
        {
            $nextSpace = strpos($string, " ", $offset);
            
            if($nextSpace !== false)
            {
                $word = substr($string, $offset, $nextSpace - $offset);
                
                $from = $offset - 20;
                
                if($from < 0)
                {
                    $from = 0;
                }
                
                $text = substr($string, $from, 50);
                $text = str_replace($word, "<b>{$word}</b>", $text);
                $offset = $nextSpace + 1;
            }
            else
            {
                $word = substr($string, $offset);
                $text = substr($string, strlen($string) - 51);
                $text = str_replace($word, "<b>{$word}</b>", $text);
                $string = "";
                $break = true;
            }
            
            if(strlen($word) > 3 && preg_match('/^[a-zA-Z]*$/', $word) !== 0)
            {
                if(!isset($words[$word]))
                {
                    $words[$word] = array(0, $text);
                }
                $words[$word][0]++;
            }
        }
        $query = SQLQuery::doMInsert(array("ref_word", "ref_item", "ref_count", "ref_text"))->table("search_refs");
        
        foreach($words as $word => $data)
        {
            $word = $this->procesWord($word);
            
            $query->mInsert(array(
                "ref_word" => $this->getID($word), 
                "ref_item" => $item, 
                "ref_count" => $data[0],
                "ref_text" => $data[1]
            ));
        }
        
        //add missing words to the db
        $this->addNewWords();
        
        //add refs
        $query->exec();
    }
    public function clearByID($id)
    {
        SQLQuery::doDelete()
            ->table("search_refs")
            ->where("ref_item", $id)
            ->exec();
    }
    public function clearByHook($hook)
    {
        $result = SQLQuery::doSelect()
            ->table("search_items")
            ->where("item_hook", $hook)
            ->exec();
        
        if($result->count() > 0)
        {
            $this->clearByID($result->getRow()->item_id);
        }
    }
    public function clearByLink($link)
    {
        $result = SQLQuery::doSelect()
            ->table("search_items")
            ->where("item_checksum", md5($link))
            ->exec();
        
        if($result->count() > 0)
        {
            $this->clearByID($result->getRow()->item_id);
        }
    }
    
    /**
     * Get the ID that comes with a word
     * 
     * @param String $word
     * @return int
     */
    private function getID($word)
    {
        if(isset($this->words2id[$word]))
        {
            return $this->words2id[$word];
        }
        else
        {
            $this->id++;
            
            $this->words2id[$word] = $this->id;
            $this->id2words[$this->id] = $word;
            $this->newWords[$this->id] = $word;
            
            return $this->id;
        }
    }
    /**
     * Add the new found words (after indexing) to the database
     */
    private function addNewWords()
    {
        if(count($this->newWords) > 0)
        {
            $query = SQLQuery::doMInsert(array("word_id", "word_word"))->table("search_words");
            
            foreach($this->newWords as $id => $word)
            {
                if(strlen($word) > 3)
                {
                    $query->mInsert(array(
                        "word_id" => $id, 
                        "word_word" => $word, 
                    ));
                }
            }
            
            $this->newWords = array();
            $query->exec();
        }
    }
    /**
     * Parse a word before proccessing
     * 
     * @param String $word
     * @return String
     */
    public function procesWord($word)
    {
        return $word;
    }
    /**
     * Search for items that are indexed
     * 
     * @param Array $words
     * @return Array
     */
    public function search($words)
    {
        $searchResults = array();
        $query = SQLQuery::doSelect()
            ->table("search_words")
            ->join("search_refs", "word_id", "ref_word", "LEFT")
            ->join("search_items", "ref_item", "item_id", "LEFT");
        
        foreach($words as $word)
        {
            $query->where("word_word", "%{$word}%", "LIKE", "OR");
        }

        $results = $query->exec();
        
        while($result = $results->getRow())
        {
            $id = $result->item_checksum;
            
            if(isset($searchResults[$id]))
            {
                $searchResults[$id]->rank += $result->ref_count;
            }
            else
            {
                $searchResults[$id] = $result;
                $searchResults[$id]->rank = $result->ref_count;
            }
        }
        
        usort($searchResults, array($this, "sort"));
        
        return $searchResults;
    }
    /**
     * Helper for sorting items based on rank
     * 
     * @param Object $a
     * @param Object $b
     * @return int
     */
    private function sort($a, $b)
    {
        if($a->rank < $b->rank)
        {
            return 1;
        }
        else if($a->rank > $b->rank)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
}