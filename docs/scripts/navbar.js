const navElement = `
<nav class="navbar navbar-expand-lg navbar-dark bg-dark">
	<div class="container">
		<a class="navbar-brand" href="/TagGang/index.html">TagGang</a>
		<div class="navbar-collapse">
		<ul class="navbar-nav">
			<li class="nav-item">
				<a class="nav-link" href="/TagGang/logs.html">Meeting Logs</a>
			</li>
			<li class="nav-item">
				<a class="nav-link" href="/TagGang/gallery.html">Gallery</a>
			</li>
			<li class="nav-item">
			</li>
		</ul>
		<ul class="navbar-nav ms-auto">
			<li class="nav-item">
				<a class="nav-link active" href="https://github.com/accellarando/TagGang">
					<img src="https://github.com/fluidicon.png" alt="GitHub" width="32" height="32">
				</a>
			</li>
		</ul>
	</div>
</nav>`;
$(document).ready(function(){
	$("body").prepend(navElement);
	// Code to mark current page as active
	let thisPage = window.location.pathname.split("/").pop();
	console.log(thisPage);
	if (thisPage === "") {
		thisPage = "index.html";
	}
	$(".navbar-nav a[href='" + thisPage + "']").addClass("active");
});
