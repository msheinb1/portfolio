'''Tests in these folders require a parameter, and I don't know how to pass that.
ex: edit(post_id), delete(post_id)'''

def test_successful_edit_page(client):
    '''Checks edit page for a post shows to authorized user.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.get('/1/edit', follow_redirects = True)
    assert len(resp.history) == 0
    assert resp.request.path == '/1/edit'

def test_edit_page_for_logged_out_user(client):
    '''Checks edit page for a post is not shown to a logged out.'''

    client.get('/logout')
    resp = client.get('/1/edit', follow_redirects = True)
    assert len(resp.history) == 1
    assert resp.request.path == '/home'
    assert b'You are not authorized to edit this post.' in resp.data

def test_edit_page_for_unauthorized_user(client):
    '''Checks edit page for a post is not shown to unauthorized user.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'bob@gmail.com',
        'user_pass': 'bees'
    })
    resp = client.get('/1/edit', follow_redirects = True)
    assert len(resp.history) == 1
    assert resp.request.path == '/home'
    assert b'You are not authorized to edit this post.' in resp.data

def test_successful_edit_function(client):
    '''Checks that a user can successfully edit their own posts.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.post('/1/edit', follow_redirects = True, data={
        'title': 'Edited Post!',
        'content': 'This post was edited!'
    })
    assert b'Post successfully editted!' in resp.data

    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_edit_function_logged_out_user(client):
    '''Checks that a logged out user cannot edit any posts.'''
    
    client.get('/logout')
    client.post('/login', data={
        'email': 'bob@gmail.com',
        'user_pass': 'bees'
    })

    resp = client.post('/1/edit', follow_redirects = True, data={
        'title': 'Edited Post!',
        'content': 'This post was edited!'
    })
    assert b'You are not authorized to edit this post.' in resp.data

    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_edit_function_unauthorized_user(client):
    '''Checks that an unauthorized user cannot edit posts that they did not write.'''
    
    client.get('/logout')

    resp = client.post('/1/edit', follow_redirects = True, data={
        'title': 'Edited Post!',
        'content': 'This post was edited!'
    })
    assert b'You are not authorized to edit this post.' in resp.data

    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_successful_delete_function(client):
    '''Checks that a user can successfully delete their own post.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'admin@msn.com',
        'user_pass': 'seed'
    })
    resp = client.post('/1/delete', follow_redirects = True)
    assert b'"Admin test post" was successfully deleted!' in resp.data
    assert b'Author: admin' not in resp.data
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_delete_function_user_not_logged_in(client):
    '''Checks that a logged out user cannot delete any post.'''

    client.get('/logout')
    resp = client.post('/1/delete', follow_redirects = True)
    assert b'You are not authorized to delete this post.' in resp.data
    assert b'Author: admin' in resp.data
    assert len(resp.history) == 1
    assert resp.request.path == '/home'

def test_delete_function_unauthorized_user(client):
    '''Checks that a logged in user cannot delete a post they did not write.'''

    client.get('/logout')
    client.post('/login', data={
        'email': 'bob@gmail.com',
        'user_pass': 'bees'
    })
    resp = client.post('/1/delete', follow_redirects = True)
    assert b'You are not authorized to delete this post.' in resp.data
    assert b'Author: admin' in resp.data
    assert len(resp.history) == 1
    assert resp.request.path == '/home'