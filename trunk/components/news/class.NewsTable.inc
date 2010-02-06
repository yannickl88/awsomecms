<?php
importField("Field");
importField("IntField");
importField("DateField");
importField("TextField");
importField("BooleanField");
importField("users.UsersField");

class NewsTable extends Table
{
    public function __construct()
    {
        parent::__construct("news");
        $this->assign("news");
        
        $this->addField(new IntField("news_id", "", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("news_title", "Title"), 1);
        $this->addField(new UsersField("news_user", "User"), 2);
        $this->addField(new DateField("news_date", "Date"), 3);
        $this->addField(new Field("news_tag", "Tag"), 4);
        $this->addField(new TextField("news_text", "Content"), 5);
        $this->setTitle("[news_title]");
        
        $extField = new IntField("news_external", "", Field::HIDE|Field::OPTIONAL);
        $extField->setDefaultValue(0);
        $this->addField($extField);
        $this->addField(new BooleanField("news_extlink", "", Field::HIDE|Field::OPTIONAL));
        
        $this->setURLs(
            Config::get('newsadmin', 'newsadmin', 'admin'),
            Config::get('newsadd', 'newsadd', 'admin'),
            Config::get('newsedit', 'newsedit', 'admin'),
            Config::get('newsdelete', 'newsdelete', 'admin')
        );
    }
    
    protected function select($query, $data = null) 
    {
        $query->orderby('news_date', "DESC");
    }
    protected function update($query, $data = null) {}
    protected function insert($query, $data = null) {}
    protected function delete($query, $data = null) {}
}