<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://www.springframework.org/tags" prefix="spring" %>
<%@ taglib uri="http://www.springframework.org/tags/form" prefix="form" %>
<html>
<head>
	<!-- Include libraries(jQuery, bootstrap) -->
	<link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" rel="stylesheet">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
	<!-- Include summernote css/js -->
	<link href="http://cdnjs.cloudflare.com/ajax/libs/summernote/0.8.9/summernote.css" rel="stylesheet">
	<script src="http://cdnjs.cloudflare.com/ajax/libs/summernote/0.8.9/summernote.js"></script>
	<!-- Title -->
	<title><spring:message code="voteboards.title"/></title>
</head>
<body>
	<div class="container" style="padding-top: 100px;">
		<h2 class="page.headder"><spring:message code="boards.write"/></h2>
		<form:form action="./" method="POST" modelAttribute="voteBoardWriteCmd" enctype="multipart/form-data">
			<table class="table table-bordered">
				<tr>
					<th><spring:message code="boards.title"/></th>
					<td><form:input path="voteBoardTitle" class="form-control"/></td>
				</tr>
				<tr>
					<th><spring:message code="boards.voteEndCondition"/></th>
					<td>
						<p><spring:message code="boards.voteLimitCnt"/>: <form:input type="number" path="voteEndCnt" value="100"/></p>
						<p><spring:message code="boards.voteLimitDate"/>: <input type="date" id="voteEndDate" value="2018-10-10"/></p>
					</td>
				</tr>
				<tr>
					<th><spring:message code="boards.image"/></th>
					<td>
						<div class="fileForm">
							<input type="file" name="voteBoardThumbnailImage" accept=".jpg, .jpeg, .png, .bmp"/>
						</div>
					</td>
				</tr>
				<tr>
					<th><spring:message code="boards.content"/></th>
					<form:hidden id="voteBoardContent" path="voteBoardContent" value="testDefault"/>
					<td><textarea class="form-control" id="summernote" name="content" placeholder="content" maxlength="140" rows="7"></textarea></td>
				</tr>
			</table>
			<input type="submit" onclick="boardWrite()" class="btn btn-primary" value="작성하기"/>
		</form:form>
	</div>
</body>
<script>
	$('#summernote').summernote({
		width: 800,
		height: 300,
		minHeight: 300,
		maxHeight: 1000,
		focus: true,
		toolbar: [
			['style', ['bold', 'italic', 'underline', 'clear']],
			['font', ['strikethrough', 'superscript', 'subscript']],
            ['fontsize', ['fontsize']],
            ['color', ['color']],
            ['para', ['ul', 'ol', 'paragraph']],
            ['height', ['height']],
            ['insert', ['link', 'table']] ]
	});
	
	function boardWrite() {
        var boardContent = $('#summernote').summernote('code');
		$('#voteBoardContent').val(boardContent);
	}
</script>
</html>