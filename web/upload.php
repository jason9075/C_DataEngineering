<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
 <html>
 <head>
 	<meta charset="UTF-8">
 	<title>上傳一個檔案</title>
 </head>
 <body>
	 <?php
	 printf("<PRE>上傳一個檔案<BR>");
	 // upload.htm表單傳送的userfile變數會傳入伺服器記憶體 $FILES['userfile']
	 printf("Name: %s <BR>", $_FILES["userfile"]["name"]);
	 // 暫存在伺服器的檔名
	 printf("Temporary Name: %s <br>",$_FILES["userfile"]["tmp_name"]);
	 // 檔案大小
	 printf("Size: %s <BR>", $_FILES["userfile"]["size"]);
	 // 檔案型態
	 printf("Type: %s <BR> <BR>",$_FILES["userfile"]["type"]);
	 // 將此暫存檔拷貝到目前執行的PHP檔案的目錄底下
	 if (copy($_FILES["userfile"]["tmp_name"],"./cgi-bin/".$_FILES["userfile"]["name"]))
	    printf("<B>File successfully copied</B><br><a href=\"http://www2.cs.ccu.edu.tw/~u99235041\">Go Back</a>");
	 else
	   printf("<B>Error: failed to copy file</B>");
	 ?>
 </body>
 </html>