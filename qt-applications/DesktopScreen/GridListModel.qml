import QtQuick 2.0

ListModel{
    property int appCount
    function fillListModel(){
            var index=0
            var m=0
            var appList = AppsModel.get_element_list()
            appCount = AppsModel.get_number_of_applications()
            listModel.clear();
            index = AppsModel.get_current_index()
            var _range = appCount - index; //remaining apps to show
            if(_range <= 18)
            {
                for(m=index; m<(index+_range); m++) //for each page
                {
                   listModel.append({"name": appList[m]["prettyname"] ,"borderColor":appList[m]["color"],"portrait":appList[m]["icon"], "app_id":appList[m]["id"]})
                }
                index = index + _range
            }
            else
            {
                for(m=index; m<index+18; m++) //for each page
                {
                   listModel.append({"name": appList[m]["prettyname"] ,"borderColor":appList[m]["color"],"portrait":appList[m]["icon"], "app_id":appList[m]["id"]})
                }
                index = index + 18
            }
            AppsModel.set_current_index(index)

     }
     function reload()
     {
           AppsModel.query_listapps()
           var appList = AppsModel.get_element_list()
           var newAppCount = AppsModel.get_number_of_applications()
           var m
           var index = AppsModel.get_current_index()
           console.log("old index#"+index +" : new app#"+ newAppCount)
           if(newAppCount>index)
           {
               var _range = newAppCount - index; //remaining apps to show
               if(_range <= 18)
               {
                   for(m=index; m<(index+_range); m++) //for each page
                   {
                      listModel.append({"name": appList[m]["prettyname"] ,"borderColor":appList[m]["color"],"portrait":appList[m]["icon"], "app_id":appList[m]["id"]})
                      console.log("appending "+m +":"+appList[m]["id"]+" - "+appList[m]["prettyname"])
                   }
                   index = index + _range
               }
               else
               {
                   for(m=index; m<index+18; m++) //for each page
                   {
                      listModel.append({"name": appList[m]["prettyname"] ,"borderColor":appList[m]["color"],"portrait":appList[m]["icon"], "app_id":appList[m]["id"]})
                   }
                   index = index + 18
               }
               AppsModel.set_current_index(index)
           }
     }

     Component.onCompleted: {
            fillListModel()
     }
}


//this function can be used to dynamically fill the grids.
//a text file can be read here to populate gridList's grids.
//the individual grids can be reached by: gridList.itemAt(i).

//in this function, you must fill the grids according to the number of elements
//per page. For instance, you should pass to the next grid for each 18 elements etc.

//uncomment the repeater above and delete the previously existing grids.
//call this function when Component.onCompleted to populate the grids.
//the anchors of the grids in gridList must be set accordingly.




