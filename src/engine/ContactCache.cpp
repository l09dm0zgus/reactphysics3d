/********************************************************************************
* ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/      *
* Copyright (c) 2010 Daniel Chappuis                                            *
*********************************************************************************
*                                                                               *
* Permission is hereby granted, free of charge, to any person obtaining a copy  *
* of this software and associated documentation files (the "Software"), to deal *
* in the Software without restriction, including without limitation the rights  *
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
* copies of the Software, and to permit persons to whom the Software is         *
* furnished to do so, subject to the following conditions:                      *
*                                                                               *
* The above copyright notice and this permission notice shall be included in    *
* all copies or substantial portions of the Software.                           *
*                                                                               *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     *
* THE SOFTWARE.                                                                 *
********************************************************************************/

// Libraries
#include "ContactCache.h"

using namespace reactphysics3d;
using namespace std;

// Constructor
ContactCache::ContactCache() {

}

// Destructor
ContactCache::~ContactCache() {
    
}

// Remove all the contact caching info of the cache
void ContactCache::clear() {
    // Clear the cache
    cache.clear();
}

// Add a new contact caching info in the cache
void ContactCache::addContactCachingInfo(ContactCachingInfo* info) {
    // Add a new entry in the cache
    cache.insert(make_pair(make_pair(info->body1, info->body2), info));
}

// Return the ContactCachingInfo (if exists) corresponding to the arguments
// If the contact pair between the two bodies in argument doesn't exist this or there is no ContactCachingInfo
// compatible (with approximatively the same position), this method returns NULL.
ContactCachingInfo* ContactCache::getContactCachingInfo(Contact* contact) const {
    double posX, posY, posZ;

    // Check if there is an entry for that pair of body in the cache
    map<pair<Body*, Body*>, ContactCachingInfo* >::const_iterator entry = cache.find(make_pair(contact->getBody1(), contact->getBody2()));
    if (entry != cache.end()) {
        ContactCachingInfo* contactInfo = (*entry).second;

        assert(contactInfo);
        assert((*entry).first.first == contact->getBody1());
        assert((*entry).first.second == contact->getBody2());

        // If the new contact and the contact caching info doesn't have the same number of contact points
        if (contact->getNbPoints() != contactInfo->positions.size()) {
            // We return NULL because, the contact doesn't match
            return NULL;
        }

        for (int i=0; i<contactInfo->positions.size(); i++) {

            // Get the position of the current contact
            posX = contact->getPoint(i).getX();
            posY = contact->getPoint(i).getY();
            posZ = contact->getPoint(i).getZ();

            // Get the position of the old contact
            Vector3D& contactPos = contactInfo->positions[i];

            // If the old contact point doesn't match the current one
            if (posX > contactPos.getX() + POSITION_TOLERANCE || posX < contactPos.getX() - POSITION_TOLERANCE ||
                posY > contactPos.getY() + POSITION_TOLERANCE || posY < contactPos.getY() - POSITION_TOLERANCE ||
                posZ > contactPos.getZ() + POSITION_TOLERANCE || posZ < contactPos.getZ() - POSITION_TOLERANCE) {

                // Return NULL
                return NULL;
            }
        }

        // The old contact positions match the current contact, therefore we return the contact caching info
        return contactInfo;
    }
    else {
        return NULL;
    }
}
