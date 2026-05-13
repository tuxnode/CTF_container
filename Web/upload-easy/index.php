<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Simple Upload Service</title>
    <script>
        function checkFile() {
            var file = document.getElementsByName('upload_file')[0].value;
            if (file == null || file == "") {
                alert("请选择要上传的文件!");
                return false;
            }
            // 前端校验：仅允许 jpg/png
            if (!/\.(jpg|jpeg|png)$/i.test(file)) {
                alert("本站只允许上传图片格式!");
                return false;
            }
            return true;
        }
    </script>
</head>
<body>
    <h1>文件备份中心</h1>
    <form action="" method="post" enctype="multipart/form-data" onsubmit="return checkFile()">
        <input type="file" name="upload_file">
        <input type="submit" name="submit" value="上传">
    </form>

    <?php
    if (isset($_POST['submit'])) {
        $upload_path = "uploads/";
        if (!file_exists($upload_path)) {
            mkdir($upload_path, 0777, true);
        }

        $file_name = $_FILES['upload_file']['name'];
        $temp_file = $_FILES['upload_file']['tmp_name'];
        
        // 后端黑名单：过滤了 .php
        $deny_ext = array(".php", ".php3", ".php4", ".php7");
        $file_ext = strrchr($file_name, '.');

        if (in_array($file_ext, $deny_ext)) {
            die("抱歉，由于安全原因，PHP 脚本被禁止上传！");
        }

        $target_path = $upload_path . $file_name;
        if (move_uploaded_file($temp_file, $target_path)) {
            echo "文件上传成功！路径: " . $target_path;
        } else {
            echo "上传失败。";
        }
    }
    ?>
</body>
</html>
