#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
// had to run 'sudo apt-get install libmysqlclient-dev' in linux terminal 
// for this header file to work
#include <mysql/mysql.h>


int main(){

   MYSQL* conn;
   conn = mysql_init(0);
   // Change third and fourth fields to your mysql username and password
   conn = mysql_real_connect(conn, "localhost", "griffin", "password", "northwind", 0, NULL, 0);
   if(conn){
       std::cout << "Connected!" << std::endl;
   } else {
       std::cout << "DATABASE CONNECTION ERROR" << std::endl;
       exit(1);
   }


    // main loop for user choice in main menu
    int userChoice = 0;

    do{
        std::cout << "Please select which action you would like to preform:" << std::endl;
        std::cout << "1. Add a customer" << std::endl;
        std::cout << "2. Add an order" << std::endl;
        std::cout << "3. Remove an order" << std::endl;
        std::cout << "4. Ship an order" << std::endl;
        std::cout << "5. Print pending orders (not shipped yet) with customer information" << std::endl;
        std::cout << "6. More Options" << std::endl;
        std::cout << "0. Exit program" << std::endl;

        std::cin >> userChoice;

        if(userChoice > 6 || userChoice < 0){
            std::cout << std::endl << "You have entered an INVAILD OPTION, program will now terminate." << std::endl << std::endl;
            return -1;
        }

        std::cout << std::endl;        

        /************************************************************** Add new customer *************************************************************/
        if(userChoice == 1){

            // Variables to store fields of customer tuple
            std::string company;
            std::string lastName;
            std::string firstName;
            std::string email;
            std::string jobTitle;
            std::string phone;
            std::stringstream ss;       

            // User inputs fields
            std::cout << "You have selected add customer:" << std::endl << std::endl;
            std::cout << "Company: ";
            std::cin.ignore(256, '\n');
            getline(std::cin, company);
            std::cout << std::endl;

            std::cout << "Last Name: ";
            getline(std::cin, lastName);
            std::cout << std::endl;

            std::cout << "First Name: ";
            getline(std::cin, firstName);
            std::cout << std::endl;

            std::cout << "Email Address: ";
            getline(std::cin, email);
            std::cout << std::endl;

            std::cout << "Job Title: ";
            getline(std::cin, jobTitle);
            std::cout << std::endl;

            std::cout << "Business Phone: ";
            getline(std::cin, phone);
            std::cout << std::endl;

            // Variables stored into string stream and then converted into c string to be passed to mysql query
            ss << "INSERT INTO Customers(Company, LastName, FirstName, Email, JobTitle, BusinessPhone) VALUES('" << company << "','" << lastName << "','" << firstName << "','" << email << "','" << jobTitle << "','" << phone << "');";
            std::string query = ss.str();
            const char* finalQ = query.c_str();
            printf("\n\n%s\n\n", finalQ);
            int error = mysql_query(conn, finalQ);

            // Error handling
            if(error == 0){
                std::cout << "Record Inserted" << std::endl << std::endl;
            } else {
                std::cout << "An error has occured, no changes made to database." << std::endl << "Error code: " << mysql_errno(conn) << std::endl << std::endl;
            }


        }





        /************************************************************** Add new Order *************************************************************/
        if(userChoice == 2){
            std::cout << "Add order" << std::endl << std::endl;

            // User enters fields for creating a new order
            std::cout << "Enter the information for your order..." << std::endl << std::endl;

            std::cout << "Employee ID: ";
            std::cin.ignore(256, '\n');
            std::string empID;
            getline(std::cin, empID);
            std::cout << std::endl;

            std::cout << "Customer ID: ";
            std::string custID;
            getline(std::cin, custID);
            std::cout << std::endl;

            std::cout << "Order Date (yyyy-mm-dd): ";
            std::string orderDate;
            getline(std::cin, orderDate);
            std::cout << std::endl;

            // Shipped is NULL

            // ShipperID is NULL

            std::cout << "Ship Name: ";
            std::string shipName;
            getline(std::cin, shipName);
            std::cout << std::endl;

            std::cout << "Ship Address: ";
            std::string shipAdd;
            getline(std::cin, shipAdd);
            std::cout << std::endl;

            std::cout << "Ship City: ";
            std::string shipCity;
            getline(std::cin, shipCity);
            std::cout << std::endl;

            std::cout << "Ship State/Province: ";
            std::string shipState;
            getline(std::cin, shipState);
            std::cout << std::endl;

            std::stringstream newOrder;

            newOrder << "INSERT INTO Orders(EmployeeID, CustomerID, OrderDate, ShipName, ShipAddress, ShipCity, ShipState, ShipZIP, ShipCountry) VALUES('" << empID << "','" << custID << "','" << orderDate << "','" << shipName << "','" << shipAdd << "','" << shipCity << "','" << shipState << "','99999', 'USA');";

            std::string newOrderQuery = newOrder.str();
            const char* newOrderCString = newOrderQuery.c_str();

            int newOrderErrno = mysql_query(conn, newOrderCString);

            // Checks if insertion was successful
            if(newOrderErrno == 0){
                
                MYSQL_RES *orderIDRES;
                MYSQL_ROW orderIDROW;

                mysql_query(conn, "SELECT OrderID FROM Orders WHERE OrderID = (SELECT MAX(OrderID) FROM Orders);");
                orderIDRES = mysql_store_result(conn);

                orderIDROW = mysql_fetch_row(orderIDRES);

                std::cout << "Enter items to be ordered..." << std::endl;

                char addMore = 'y';

                // Add (multiple) items to order detials
                do {
                    std::cout << "Product ID: ";
                    std::string productID;
                    getline(std::cin, productID);
                    std::cout << std::endl;

                    // Check for valid product id
                    MYSQL_RES *validPID;
                    MYSQL_ROW pidROW;

                    std::stringstream pidQuery;

                    pidQuery << "SELECT Discontinued FROM Products WHERE ID = " << productID << ";";
                    std::string pidQueryString = pidQuery.str();
                    mysql_query(conn, pidQueryString.c_str());

                    validPID = mysql_store_result(conn);
                    pidROW = mysql_fetch_row(validPID);

                    if(pidROW[0] != "1"){
                        std::cout << "Quantity: ";
                        std::string quantity;
                        getline(std::cin,  quantity);
                        std::cout << std::endl;

                        std::stringstream newOrderDetail;

                        newOrderDetail << "INSERT INTO Order_Details(OrderID, ProductID, Quantity, StatusID) VALUES('" << orderIDROW[0] << "','" << productID << "','" << quantity << "', 2);";
                        std::string newOrderDetialString = newOrderDetail.str();

                        int orderDetialErrno = mysql_query(conn, newOrderDetialString.c_str());
                        
                        if(orderDetialErrno == 0) std::cout <<  "Insertion Success!" << std::endl;
                        else std::cout << "Error: " << mysql_errno(conn);
                        newOrderDetail.str("");
                    } else {
                        std::cout << "You have selected a discontinued item." << std::endl;
                    }

                    // Ask for new loop
                    std::cout << "Would you like to add another item? (y/n): ";
                    std::cin >> addMore;
                    std::cout << std::endl;                    
                    std::cin.ignore();
                } while (addMore == 'y'); 

            } 
            // Stops update of order details if origial insertions fails
            else {
                std::cout << "Failed to add new order." << std::endl << "Error Number: " << mysql_errno(conn) << std::endl;
                std::cout << "SQL Statement: ";
                printf("%s\n", newOrderCString);
            }
        }

        /************************************************************** Remove order *************************************************************/
        if(userChoice == 3){
            std::cout << "Remove order" << std::endl << std::endl;

            // User Enters fields
            std::cout << "Enter the Order ID for the order you would like to remove: ";
            std::string orderID;
            std::cin >> orderID;
            std::cout << std::endl;

            std::stringstream idStream;
            std::stringstream rollbackData;

            rollbackData << "SELECT * FROM Order_Details WHERE OrderID = " << orderID << ";";
            std::string rollbackString = rollbackData.str();
            const char* rollbackCString = rollbackString.c_str();

            mysql_query(conn, rollbackCString);
            MYSQL_RES *rollbackRes;
            rollbackRes = mysql_store_result(conn);

            // Convert from string stream to c string
            idStream << "DELETE FROM Order_Details WHERE OrderID = " << orderID << ";";

            std::string query2 = idStream.str();
            const char* finalQ2 = query2.c_str();

            int OrdersDetailsDeleteError = mysql_query(conn, finalQ2);

            if(OrdersDetailsDeleteError == 0){
                // Delete from Order Details
                std::stringstream idStream2;
                idStream2 << "DELETE FROM Orders WHERE OrderID = " << orderID << ";";
                std::string query3 = idStream2.str();
                const char* finalQ3 = query3.c_str();
                int OrdersDeleteError = mysql_query(conn, finalQ3);

                if(OrdersDeleteError != 0){
                    // Roll back to not leave parent childless
                    std::cout << "Error Number: " << mysql_errno(conn) << std::endl;
                    std::cout << "Error deleting Order ID: " << orderID << " from Orders. Attemping to roll back first deletion" << std::endl;

                    std::stringstream insertRollback;
                    insertRollback << "INSERT INTO Order_Details(OrderID, EmployeeID, CustomerID, OrderDate, ShippedDate, ShipperID, ShipName, ShipAddress, ShipCity, ShipState, ShipZIP, ShipCountry, ShippingFee, Taxes, PaymentType, PaidDate, Notes, TaxRate, TaxStatus, StatusID) VALUES(";

                    MYSQL_ROW rollbackRow;

                    rollbackRow = mysql_fetch_row(rollbackRes);

                    for(int i = 0; i < mysql_num_fields(rollbackRes); i++){
                        if(rollbackRow != NULL){
                            insertRollback << "\"" << rollbackRow[i] << "\",";
                        }
                        else
                            insertRollback << "\" NULL \",";
                    }

                    std::string rollString = insertRollback.str();
                    const char* rollCString = rollString.c_str();

                    int rollbackSuccess = mysql_query(conn, rollCString);

                    if(rollbackSuccess == 0) std::cout << "Rollback Success!" << std::endl;

                    else std::cout << "Rollback failed, loss of OrderID: " << orderID << " has occured in Order_Details." << std::endl;
 
                }

            }

            // Triggers if deletion from Order_Details fails
            else{
                std::cout << "Deletion of Order ID: " << orderID << " has failed, no changes made to database." << std::endl;
                std::cout << "Error Number: " << mysql_errno(conn) << std::endl << std::endl;
            }

            mysql_free_result(rollbackRes);

        }

        /************************************************************** Ship Order *************************************************************/
        if(userChoice == 4){
            std::cout << "Ship order" << std::endl << std::endl;

            std::cout << "Enter the ID of the order you would like to ship: ";
            std::cin.ignore(256, '\n');
            std::string shipOrderID;
            getline(std::cin, shipOrderID);
            std::cout << std::endl;

            std::stringstream orderProductIDs;

            orderProductIDs << "SELECT ProductID, Quantity FROM Order_Details WHERE OrderID = " << shipOrderID << ";";
            std::string orderProductIDsString = orderProductIDs.str();

            MYSQL_RES *shipOrderIDResult;
            MYSQL_ROW shipOrderIDRow;

            mysql_query(conn, orderProductIDsString.c_str());
            shipOrderIDResult = mysql_store_result(conn);

            bool inStock = true;
            MYSQL_RES *prodInfoRes;
            MYSQL_ROW prodInfoRow;

            // Check quantity ordered against inventory
            while(shipOrderIDRow = mysql_fetch_row(shipOrderIDResult)){

                std::stringstream prodInfoQueryStream;
                prodInfoQueryStream << "SELECT Quantity FROM Inventory_Transactions WHERE ProductID = " << shipOrderIDRow[0] << " ORDER BY Quantity DESC;";
                std::string prodInfoQueryString = prodInfoQueryStream.str();
                int PIDErrno = mysql_query(conn, prodInfoQueryString.c_str());

                prodInfoRes = mysql_store_result(conn);
                prodInfoRow = mysql_fetch_row(prodInfoRes);

                


                // Crashes if ProductID does not exist in Inventory_Transactions :(
                if(mysql_fetch_fields(prodInfoRes) != 0){
                    if(atoi(prodInfoRow[0]) < atoi(shipOrderIDRow[1])){
                        inStock = false;
                        break;
                    } else {
                        std::stringstream updateStream;
                        updateStream << "UPDATE Inventory_Transactions SET Quantity = " << atoi(prodInfoRow[0]) - atoi(shipOrderIDRow[1]) << " WHERE ProductID = " << shipOrderIDRow[0] << ";";

                        std::string updateString = updateStream.str();
                        mysql_query(conn, updateString.c_str());

                    }
                } else {
                    inStock = false;
                    break;
                }
                prodInfoQueryStream.str("");
            }

            if(inStock == true){
                std::cout << "Items are instock" <<std::endl;

                std::cout << "Enter Shipping date (yyyy-mm-dd): ";
                std::string shipDate;
                getline(std::cin, shipDate);
                std::cout << std::endl;

                std::cout << "Enter Ship ID: ";
                std::string shipID;
                getline(std::cin, shipID);
                std::cout << std::endl;

                std::cout << "Enter Shipping Fee: ";
                std::string shipFee;
                getline(std::cin, shipFee);
                std::cout << std::endl;

                std::stringstream orderUpdateStream;
                orderUpdateStream << "UPDATE Orders SET ShippedDate = '" << shipDate << "', ShipperID = '" << shipID << "', ShippingFee = '" << shipFee << "' WHERE OrderID = " << shipOrderID << ";";
                std::string orderUpdateString = orderUpdateStream.str();
                mysql_query(conn, orderUpdateString.c_str());

                mysql_query(conn, orderProductIDsString.c_str());
                shipOrderIDResult = mysql_store_result(conn);

                
                


                


            } else {
                std::cout << "Item is out of stock." << std::endl;
            }

            mysql_free_result(prodInfoRes);
            mysql_free_result(shipOrderIDResult);
        } // End ship order


        /************************************************************** Print pending orders *************************************************************/
        if(userChoice == 5){

            MYSQL_RES *result;
            MYSQL_ROW row;

            // Query ran to find unshipped orders
            mysql_query(conn, "SELECT * FROM Orders WHERE ShippedDate IS NULL ORDER BY OrderDate;");
            result = mysql_store_result(conn);
            
            //Prints if no orders found
            if(result == NULL) std::cout << "No pending orders found." << std::endl;

            else
                std::cout << "Order ID\tEmployee ID\tCustomer ID\tOrder Date\tShipped Date\tShip Name\tShip Address\tShip City\tShip Zip\tShip Country\tShipping Fee\tTaxes\tPayment Type\tPaid Date\tNotes\tTax Rate\tTax Status\tStatus ID" << std::endl;

            // Loop over row and fields to print result or query
            while(row = mysql_fetch_row(result)){
                for(int i = 0; i < mysql_num_fields(result); i++)
                {
                    if(row[i] != NULL)
                        std::cout << row[i] << "\t";

                    else
                        std::cout << "NULL" << "\t";
                }
                std::cout << std::endl;
            }

            // Free result of query
            if(result != NULL) mysql_free_result(result);
            std::cout << std::endl;

        }

        /************************************************************** More features *************************************************************/
        if(userChoice == 6){
            std::cout << "More Options" << std::endl << std::endl;
            std::cout << "Not yet implemented, check back later :)" << std::endl << std::endl;
        }
        
        


    } while (userChoice != 0);
    

    mysql_close(conn);

    if(userChoice == 0) std::cout << "Bye!" <<  std::endl << std::endl;

    return 0;
}