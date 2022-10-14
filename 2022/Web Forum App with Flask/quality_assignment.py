'''Description: This is the module that handles the backend code for
 the quality_assignment blog app.

Pylint results:
-------------------------------------------------------------------
Your code has been rated at 10.00/10 (previous run: 9.71/10, +0.29)
'''

import sqlite3
import re
from flask import Flask, render_template, request, redirect, url_for, session, abort, flash

app = Flask(__name__)
app.secret_key = b'4h@H#IPr2bh3qj23G3@3'
app.config['DB_LOCATION'] = 'sqlite3-database.db'
ERROR = 'error'
INFO = 'info'

def get_db_connection():
    '''Description: Opens connection to database.'''

    conn = sqlite3.connect(app.config['DB_LOCATION'])
    conn.row_factory = sqlite3.Row
    return conn

def get_post(post_id):
    '''Argument: post_id as integer

    Description: Connects to the database, searches for the post in the database,
    and returns the post.

    Error: If no post is found, aborts with error code 404.'''
    conn = get_db_connection()
    post = conn.execute('SELECT * FROM posts WHERE id = ?', (post_id,)).fetchone()
    conn.close()
    if post is None:
        abort(404)
    return post

@app.route('/<int:id>/edit', methods=('GET', 'POST'))
def edit(post_id):
    '''Argument: post_id as integer

    Description: Allows a user to edit a post, then redirects to the home page.

    Error: If no post is found, aborts with error code 404 from get_post(post_id).
    If the user currently logged in is not the same as the author of the post, flashes an error.'''

    post = get_post(post_id)
    if 'username' in session and session['username'] == post['author']:
        if request.method == 'POST':
            title = request.form['title']
            content = request.form['content']

            if not title:
                flash('Title is required!', ERROR)

            elif not content:
                flash('Content is required!', ERROR)

            else:
                conn = get_db_connection()
                conn.execute('UPDATE posts SET title = ?, content = ?'
                            ' WHERE id = ?',
                            (title, content, id))
                conn.commit()
                conn.close()
                flash('Post successfully editted!', INFO)
                return redirect(url_for('home'))

        return render_template('edit.html', name = session.get('username'), post=post)

    flash('You are not authorized to edit this post.', ERROR)
    return redirect(url_for('home'))

@app.route('/<int:id>/delete', methods=('POST',))
def delete(post_id):
    '''Argument: post_id as integer

    Description: Allows a user to delete a post, then redirects to the home page.

    Error: If no post is found, aborts with error code 404 from get_post(post_id).
    If the user currently logged in is not the same as the author of the post, flashes an error.

    Additional Comments: The if statement handles case where if you go to edit page
    as authorized user, go to new tab, logout, then go back to first tab and try to click delete.

    Created post_title variable and assigned post['title'] to it because
    f-string didn't like f'{post['title']} was successfully deleted!' '''

    post = get_post(post_id)
    if 'username' in session and session['username'] == post['author']:
        title = post['title']
        conn = get_db_connection()
        conn.execute('DELETE FROM posts WHERE id = ?', (id,))
        conn.commit()
        conn.close()
        flash(f'"{title}" was successfully deleted!', INFO)
        return redirect(url_for('home'))

    flash('You are not authorized to delete this post.', ERROR)
    return redirect(url_for('home'))

@app.route('/')
def home_alt():
    '''Description: Routes requests to show / to redirect to /home.'''

    return redirect(url_for('home'))

@app.route('/home')
def home():
    '''Description: Displays all posts from newest to oldest.'''

    conn = get_db_connection()
    posts = conn.execute('SELECT * FROM posts ORDER BY created DESC').fetchall()
    conn.close()

    return render_template('main.html', name = session.get('username'), posts=posts)

@app.route('/<username>')
def show_posts(username):
    '''Argument: username as string

    Description: Displays all posts by a specific user from newest to oldest.'''

    conn = get_db_connection()
    posts = conn.execute('SELECT * FROM posts WHERE author = ? ORDER BY created DESC',
                     (username,)).fetchall()
    conn.close()

    return render_template('main.html', name = session.get('username'), posts=posts)

@app.route('/about')
def about():
    '''Description: Displays all registered users. If you click on a username you can
    see all their posts. If logged in as the admin, additionally shows the emails of each user.'''

    conn = get_db_connection()
    users = conn.execute('SELECT * FROM users').fetchall()
    conn.close()

    return render_template('about.html', name = session.get('username'), users = users)

@app.route('/create', methods=['GET', 'POST'])
def create():
    '''Description: GET - Shows the form to allow creation of posts.
    POST - Creates a Post, then redirects to the home page.
    Error: POST - If the user is not logged in, flashes an error.'''

    if 'username' in session:
        if request.method == 'POST':
            title = request.form['title']
            content = request.form['content']
            author = session['username']

            if not title:
                flash('Title is required!', ERROR)
            elif not content:
                flash('Content is required!', ERROR)
            else:
                conn = get_db_connection()
                conn.execute('INSERT INTO posts (title, author, content) VALUES (?, ?, ?)',
                            (title, author, content))
                conn.commit()
                conn.close()
                flash(f'Post "{title}" successfully created.', INFO)
                return redirect(url_for('home'))
    else:
        flash('Must be logged in to create posts.', ERROR)

    return render_template('create.html', name = session.get('username'))

@app.route('/login', methods=['GET', 'POST'])
def login():
    '''Description: GET - Shows the login page.
    POST - Logs in the user, redirects the user to the home page (regular user)
    or the about page (admin).

    Error: POST - If the user is not registered, flashes error.
    POST - If the password is incorrect, flashes error.
    POST - If the user is already logged in, flashes error.'''

    if request.method == 'POST':
        conn = get_db_connection()
        user = conn.execute('SELECT * FROM users WHERE email = ?',
                        (request.form['email'],)).fetchone()
        conn.close()

        if not user:
            flash('User does not exist. Please register.', ERROR)
            return render_template('login.html', name = session.get('username'))

        user_pass = request.form['user_pass']
        if user_pass != user['user_pass']:
            flash('Incorrect password.', ERROR)
            return render_template('login.html', name = session.get('username'))

        session['username'] = user['username']
        flash('Successfully logged in!', INFO)
        if session['username'] == 'admin':
            return redirect(url_for('about'))

        return redirect(url_for('home'))

    if session.get('username'):
        flash('Already logged in. Please log out first.', ERROR)
        return redirect(url_for('home'))

    return render_template('login.html', name = session.get('username'))

@app.route('/logout')
def logout():
    '''Description: Logs out the user, redirects to home page.

    Error: If the user is not logged in, flashes error.'''

    if session.get('username') is None:
        flash('You are not logged in!', ERROR)
    # remove the username from the session if it's there
    session.pop('username', None)
    return redirect(url_for('home'))

#After registering username + password, redirect to login
@app.route('/register', methods=['GET', 'POST'])
def register():
    '''Description: GET - Shows the register page.
    POST - Registers the user.

    Error: POST - If the username, email, or password fields are empty, flashes error.
    POST - If the username or email already exists in the users table, flashes error.'''

    if request.method == 'POST':
        username = request.form['username']
        email = request.form['email']
        user_pass = request.form['user_pass']

        if not username:
            flash('Username field cannot be blank.', ERROR)
            return render_template('register.html', name = session.get('username'))
        if not email:
            flash('Email field cannot be blank.', ERROR)
            return render_template('register.html', name = session.get('username'))
        if not user_pass:
            flash('Password field cannot be blank.', ERROR)
            return render_template('register.html', name = session.get('username'))

        #Probably going to run into email formatting issues at some point.
        if not re.search('^[^@]*@{1}[^@.]*[.]{1}[^@.]*$', email):
            flash('Incorrect email format.', ERROR)
            return render_template('register.html', name = session.get('username'))

        try:
            conn = get_db_connection()
            conn.execute('INSERT INTO users (username, email, user_pass) VALUES (?, ?, ?)',
                            (username, email, user_pass))
            conn.commit()
            conn.close()
            flash('Successfully registered!', INFO)
            return redirect(url_for('login'))
        except sqlite3.IntegrityError as exception_error:
            error_string = str(exception_error)
            if error_string == 'UNIQUE constraint failed: users.username':
                flash(f'Username {username} already registered.', ERROR)
            elif error_string == 'UNIQUE constraint failed: users.email':
                flash(f'Email {email} already registered.', ERROR)
            else:
                flash(f'{error_string}', ERROR)

    return render_template('register.html', name = session.get('username'))
