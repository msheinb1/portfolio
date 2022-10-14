'''Test list 
Login valid user, redirect to <user>/posts with flash -> Success
Login invalid user -> Fail
Login admin redirect to display users + emails -> Success
valid email (string) -> Succeed
Invalid email/user -> Fail
no @ in email field -> Fail

Register valid user -> Success
Register invalid user (like shared username) -> Fail

Logut when logged in -> Success
Logout when not logged in -> Fail

Can create post with empty title -> Fail
Can create post with empty body -> Fail
Can create post when logged in without empty title or body -> Succeed

Edit post, logged in to user who made post -> Success
Edit post, logged in to different user -> Fail
Edit post, not logged in -> Fail

Delete post, logged in to user who made post -> Success
Delete post, logged in to different user -> Fail
Delete post, not logged in -> Fail

Views:
Home Page/Main
Displays all posts -> Succeed

About
Displays usernames regardless of logged in or not -> Success
Displays emails if logged into regular user -> Fail
Displays emails if logged into admin -> Success

Create
Create post, logged in -> Success
Create post, not logged in -> Fail

Edit
Can see edit window when logged into correct user -> Succeed
Can see edit window when logged into incorrect user -> Fail

Login - uses email only
Go to login page while already logged in -> Fail

Register
Unique username -> Succeed
Duplicate username -> Fail
Correct email (string) -> Succeed
Incorrect email format -> Fail
Unique email -> Succeed
Duplicate email -> Fail
Password field displays * chars -> Succeed

/<username>
Displays all posts by <username> -> Succeed

'''

