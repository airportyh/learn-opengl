# Learning OpenGL

I am learning OpenGL and these are some of my exercise programs and the
build script I used to compile them on my Mac. I've used Homebrew to
install the required libraries:

```
brew install glew
brew install glfw
brew install glm
```

It hard-links the location of the glm include files at: `/usr/local/Cellar/glm/0.9.9.5/include/glm/`
in the `build` script.
You may need to change that if your location is different. You can find out via:

```
brew list glm
```

## Resources

These are the resources I am currently using to learn:

* [Tom Dalling's OpenGL Series](https://www.tomdalling.com/blog/category/modern-opengl/)
* [Casey Muratori's Handmade Hero's Series](https://handmadehero.org/) - search YouTube for "Handmade Hero OpenGL"
* [Alexander Overoovde's OpenGL Series](https://open.gl/)
* [Joe Durian's OpenGL Series](http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-1:-The-Graphics-Pipeline.html)
