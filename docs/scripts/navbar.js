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
				<a class="nav-link" href="/TagGang/schedule.html">Schedule</a>
			</li>
			<li class="nav-item dropdown">
				<a class="nav-link dropdown-toggle" data-toggle="dropdown" href="#" role="button" aria-haspopup="true" aria-expanded="false">Hardware</a>
				<div class="dropdown-menu">
					<a class="dropdown-item" href="/TagGang/hardware/plotter.html">Plotter</a>
					<a class="dropdown-item" href="/TagGang/hardware/hat.html">CNC Pi HAT</a>
				</div>
			</li>
			<li class="nav-item dropdown">
				<a class="nav-link dropdown-toggle" data-toggle="dropdown" href="#" role="button" aria-haspopup="true" aria-expanded="false">Software</a>
				<div class="dropdown-menu">
					<a class="dropdown-item" href="/TagGang/software/gui.html">GUI</a>
					<a class="dropdown-item" href="/TagGang/software/kinect.html">Kinect UI</a>
				</div>
			</li>
			<li class="nav-item">
				<a class="nav-link" href="/TagGang/members.html">Team Members</a>
			</li>
			<li class="nav-item">
				<a class="nav-link" href="/TagGang/gallery.html">Gallery</a>
			</li>
			<li class="nav-item">
				<a class="nav-link" href="/TagGang/notes/resources.html">Resources</a>
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

	$(".dropdown").click(function(){
		$(this).dropdown();
	};

	// Code to mark current page as active
	let thisPage = window.location.pathname.split("/").pop();
	if (thisPage === "") {
		thisPage = "index.html";
	}
	$(".navbar-nav a[href='" + thisPage + "']").addClass("active");
});
