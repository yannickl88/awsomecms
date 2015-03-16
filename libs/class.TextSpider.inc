<?php
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
    public function index($string, $link, $title, $hook, $text)
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
        $item = -1;

        $result = SQLQuery::doSelect()
            ->table("search_items")
            ->where("item_hook", $hook)
            ->exec();

        if($result->count() > 0)
        {
            $item = $result->getRow()->item_id;
            $this->clearByID($item);

            SQLQuery::doUpdate()
                ->table("search_items")
                ->update("item_link", $link)
                ->update("item_title", serialize($title))
                ->update("item_checksum", md5($link))
                ->update("item_hook", $hook)
                ->update("item_text", serialize($text))
                ->where("item_id", $item)
                ->exec();
        }
        else
        {
            $item = SQLQuery::doInsert()
                ->table("search_items")
                ->insert("item_link", $link)
                ->insert("item_title", serialize($title))
                ->insert("item_checksum", md5($link))
                ->insert("item_hook", $hook)
                ->insert("item_text", serialize($text))
                ->exec()
                ->insertID();
        }

        if(is_array($string))
            $string = implode(" ", $string);

        $string = trim(preg_replace(array('/(\s+)/', '/(\[.*?\])/', '/(<.*?>)/', '/({.*?})/'), " ", str_replace(array(".", ",", "'", "\"", "\n", "\r", "“"), " ", strtolower($string))));
        $words = array();
        $offset = 0;
        $break = false;

        while(!$break)
        {
            $nextSpace = strpos($string, " ", $offset);

            if($nextSpace !== false)
            {
                $word = $this->procesWord(substr($string, $offset, $nextSpace - $offset));

                $from = $offset - 20;

                if($from < 0)
                {
                    $from = 0;
                }

                $offset = $nextSpace + 1;
            }
            else
            {
                $word = $this->procesWord(substr($string, $offset));
                $string = "";
                $break = true;
            }

            if(preg_match('/^[a-zA-Z0-9]*$/', $word) !== 0 && strlen($word) > 1)
            {
                if(!isset($words[$word]))
                {
                    $words[$word] = 0;
                }
                $words[$word]++;
            }
        }
        $query = SQLQuery::doMInsert(array("ref_word", "ref_item", "ref_count"))->table("search_refs");

        foreach($words as $word => $data)
        {
            $query->mInsert(array(
                "ref_word" => $this->getID($word),
                "ref_item" => $item,
                "ref_count" => $data,
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
                $query->mInsert(array(
                    "word_id" => $id,
                    "word_word" => $word,
                ));
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
        return PorterStemmer::Stem($word);
    }
    /**
     * Search for items that are indexed
     *
     * @param Array $words
     * @return Array
     */
    public function search($words, $limit = -1)
    {
        $searchResults = array();
        $query = SQLQuery::doSelect()
            ->select("COUNT(word_id) AS rating")
            ->select("search_items.*")
            ->table("search_words")
            ->join("search_refs", "word_id", "ref_word", "LEFT")
            ->join("search_items", "ref_item", "item_id", "INNER")
            ->groupBy("search_items.item_id")
            ->orderBy("rating", "DESC");

        foreach($words as $word)
        {
            $query->where("word_word", "%".PorterStemmer::Stem($word)."%", "LIKE", "OR");
        }

        if($limit >= 0)
            $query->limit($limit[0], $limit[1]);

        $searchResults = $query->exec()->getRows();

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