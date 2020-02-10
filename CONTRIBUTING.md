# Contributing

This document contains guidelines for contributing to ETS2 Job Logger.

##### Table of Contents

- [Reporting issues](#reporting-issues)
    - [Bug report](#bug-report)
    - [Feature request](#feature-request)
- [Contributing as developer](#contributing-as-developer)
    - [Issues](#issues)
    - [Git commit messages](#git-commit-messages)
    - [Submitting pull request](#submitting-pull-request)
    - [Reviewing pull request](#reviewing-pull-request)
    - [Coding conventions](#coding-conventions)
        - [C++ style](#c-style)
        - [CMake](#cmake)

## Reporting issues

Before creating an issue make sure similar issue doesn't exist.

Github supports Markdown so take advantage of it when appropriate to make your
issue more readable. For quick reference see for example [Mastering Markdown](https://guides.github.com/features/mastering-markdown/).

Instead pasting long backtraces and build logs and so forth prefer [pastebin.com](https://pastebin.com/).

### Bug report

A good title should quickly identify the issue:

- Good: "Plugin crashes when player gets fined"
- Bad: "Game crashes"

Include steps to reproduce the issue. If you can't reproduce the problem then
there is probably no use to reporting it. Good example of "how to reproduce":

```
1. Start new game
2. Take any Quick Job
3. Crash with a vehicle to receive a fine

Expected result: Fine is sent to the API
Actual result: Game crashes to exception ...

Operating System: Linux, Windows, MacOS
```

If `Wine` or `Proton` was used then state it's version. If it is not stated
then it is assumed it was not being used.

Additional information such as link to backtrace and so forth can be linked
at the bottom of the issue.

### Feature request

A good title should simply and succinctly state the intention:

- Good: "Option to disable Truck -endpoint"
- Bad: "Huge API overhead"

Describe your use case or the problem and then suggest the solution.

- Good:

```
I have not implemented Truck -endpoint in my API because I only want
to track jobs. Being not able to disable sending truck's positional to
Truck -endpoint creates unnecessary overhead on my server.

API could be queried of which endpoints it supports and then requests
would be sent only to the enabled endpoints.
```

- Bad:

```
Disable Truck -endpoint.
```

## Contributing as developer

### Issues

Check for open [bugs](https://github.com/Lihis/ets2-job-logger/labels/bug),
[enhancements](https://github.com/Lihis/ets2-job-logger/labels/enhancement)
and others. If there is open [milestone](https://github.com/Lihis/ets2-job-logger/milestones)
try to prioritize issues assigned to it.

### Git commit messages

Good commit message communicates about the context of the changes. This helps
others and even your future self when you look back in the history in the
future.

Structure your commit messages like this (from [5.2 Distributed Git - Contributing to a Project](https://git-scm.com/book/en/v2/Distributed-Git-Contributing-to-a-Project)):

```
Capitalized, short (50 chars or less) summary

More detailed explanatory text, if necessary.  Wrap it to about 72
characters or so.  In some contexts, the first line is treated as the
subject of an email and the rest of the text as the body.  The blank
line separating the summary from the body is critical (unless you omit
the body entirely); tools like rebase can get confused if you run the
two together.

Further paragraphs come after blank lines.

- Bullet points are okay, too

- Typically a hyphen or asterisk is used for the bullet, followed by a
  single space, with blank lines in between, but conventions vary here

- Use a hanging indent
```

Chris Beams [How to Write a Git Commit Message](https://chris.beams.io/posts/git-commit/)
can be worth of reading.

### Submitting pull request

If you're submitting a pull request:

- Check your code follows [Coding conventions](#coding-conventions).
- Check you have written good commit messages (see [Git commit messages](#git-commit-messages)).
- Include a reference to any issue the pull request is intended to address.
  - Referenced issues gets automatically closed when the pull request is merged.
  - See Github's guide of [Linking a pull request to an issue using a keyword](https://help.github.com/en/github/managing-your-work-on-github/linking-a-pull-request-to-an-issue#linking-a-pull-request-to-an-issue-using-a-keyword).
- Try to keep unrelated changes in separate pull request.
- If relative, check you have updated API documentation.

### Reviewing pull request

Pull request should be reviewed by many people as possible. New code should
follow [Coding conventions](#coding-conventions). If code introduces changes
to the API the documentation should have been updated to reflect the changes.

If you have reviewed a pull request and think it should be accepted, submit
review with status `Approved` and state that the changes look good for you and
add any additional notes you may have. If the changes contains issues which
should be addressed before the pull request can be merged submit review with
status `Request changes`. Use status `Comment` when you have general feedback
and don't yet want to explicitly approve or request changes.

Continuous integration should ensure that the changes do not cause build
errors in different platforms. However the changes should be tested on
supported platforms (Linux, MacOS, Windows) for example when adding new library
to ensure ETS2 Job Logger continues to work.

### Coding conventions

#### C++ style

Code should follow [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
with following exceptions:

- Target C++11.
- Indent with 4 spaces.
- Always use braces for `if`, `else`, `for`, `do` and `while`.
- Class Data Members are prefixed with `m_`.

#### CMake

- Indent with 4 spaces.
- Functions in lowercase.
    - e.g. `message(STATUS "Hello World")`
- Variables in uppercase.
    - e.g. `set(FOOVAR FALSE)`
- Use empty commands for `else()`, `endif()`, `endforeach()`, `endfunction()`,
  `endmacro()` and `endwhile()`.

Example:

```cmake
set(FOOVAR TRUE)
if(FOOVAR)
    message(STATUS "FOOVAR is true")
else()
    message(STATUS "FOOVAR is false")
endif()
```
