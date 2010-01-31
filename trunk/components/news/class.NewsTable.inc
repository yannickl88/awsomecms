<?php
importField("Field");
importField("IntField");
importField("DateField");
importField("TextField");
importField("BooleanField");

class NewsTable extends Table
{
    public function __construct()
    {
        parent::__construct("news");
        $this->assign("news");
        
        $this->addField(new IntField("news_id", "", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("news_title", "Title"), 1);
        $this->addField(new IntField("news_user", "User"), 2);
        $this->addField(new DateField("news_date", "Date"), 3);
        $this->addField(new Field("news_tag", "Tag"), 4);
        $this->addField(new TextField("news_text", "Content"), 5);
        $this->addField(new IntField("news_external", "", Field::HIDE));
        $this->addField(new BooleanField("news_extlink", "", Field::HIDE));
    }
    
    protected function select($query, $data = array()) {}
    protected function update($query, $data = array()) {}
    protected function insert($query, $data = array()) {}
    protected function delete($query, $data = array()) {}
}