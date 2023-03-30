Static files like images, CSS, JavaScript, fonts, etc should be stored within the public directory.

Public files are copied to the root of the out directory when built. They can therefore be accessed through template files with the out dir as the root.<br>
Example: `<link rel=\"stylesheet\" href=\"/css/main.css\" />` will find the CSS file `<out_dir>/css/main.css`.

*Note: Avoid relative routing when accessing static files, as it will not be the same path when built.*