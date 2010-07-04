<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package lib
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
    }
    /**
     * Index a text block, the found words and matches are stored in the database
     * 
     * @param String $string
     * @param int $link
     * @param String $hook
     */
    public function index($string, $link, $hook)
    {
        //clear the table of the current indexes
        SQLQuery::doDelete()
            ->table("search_refs")
            ->where("ref_link", $link)
            ->where("ref_hook", $hook)
            ->exec();
        
        $string = preg_replace("(\s+)", " ", str_replace(array(".", ",", "'", "\"", "\n", "\r", "“"), " ", strtolower($string)));
        $textArr = explode(" ", $string);
        $words = array_count_values($textArr);
        
        $query = SQLQuery::doMInsert(array("ref_word", "ref_link", "ref_hook", "ref_count"))->table("search_refs");
        
        foreach($words as $word => $count)
        {
            $word = $this->procesWord($word);
            
            if(strlen($word) > 3)
            {
                $query->mInsert(array(
                    "ref_word" => $this->getID($word), 
                    "ref_link" => $link, 
                    "ref_hook" => $hook, 
                    "ref_count" => $count
                ));
            }
        }
        
        //add missing words to the db
        $this->addNewWords();
        
        //add refs
        $query->exec();
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
            ->join("search_refs", "word_id", "ref_word", "LEFT");
        
        foreach($words as $word)
        {
            $query->where("word_word", "%{$word}%", "LIKE", "OR");
        }

        $results = $query->exec();
        
        while($result = $results->getRow())
        {
            $id = $result->ref_hook."-".$result->ref_link;
            
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