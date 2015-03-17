<h2>{"collectionoverview"|text}</h2>
<div id="collectionOverviewWrapper">
    <a href="#" class="addMachine">
        <img src="/img/icons/add.png" alt="add new machine" /> Add new Machine
    </a>
    <div class="machines">
    </div>
</div>
<script type="text/javascript">
    function collection_addMachine(e) {
        var html = $('<div class="machine"></div>');
        var link = $('<a href="#" class="icon"><img src="/img/icons/add.png" alt="add model" />Add Model</a>');

        link.click(collection_addModel);
        
        html.append('<input type="text" value="Buldozer" />');
        html.append('<div class="models"></div>');
        html.append(link);
        html.append('<div class="clear"></div>');
        $("#collectionOverviewWrapper .machines").append(html);

        e.stopPropagation();
        return false;
    }
    function collection_addModel(e) {
        var html = $('<div class="model"></div>');
        var link = $('<a href="#" class="icon"><img src="/img/icons/add.png" alt="add type" />Add Type</a>');

        link.click(collection_addType);
        
        html.append('<input type="text" value="D6" />');
        html.append('<div class="types"></div>');
        html.append(link);
        html.append('<div class="clear"></div>');
        $(this).parent().find(".models").append(html);

        e.stopPropagation();
        return false;
    }
    function collection_addType(e) {
        var html = $('<div class="type"></div>');
        var link = $('<a href="#" class="icon"><img src="/img/icons/add.png" alt="add brand" /> Add Brand</a>');

        link.click(function(e) {
            e.stopPropagation();
            return false;
        });
        
        html.append('<input type="text" value="D6T" />');
        html.append(link);
        html.append('<div class="brands"></div>');
        html.append('<div class="clear"></div>');
        $(this).parent().find(".types").append(html);

        e.stopPropagation();
        return false;

    }
    $().ready(function(e) {
        $("#collectionOverviewWrapper a.addMachine").click(collection_addMachine);
    });
</script>