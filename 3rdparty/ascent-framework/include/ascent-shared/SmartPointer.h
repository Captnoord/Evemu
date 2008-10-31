/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _SP_GARBAGE_COLLECTOR_H
#define _SP_GARBAGE_COLLECTOR_H

//Reference based smart pointers
//Save while using hahsmaps etc for reference increasing.
//shared pointers will auto delete themselves but even with a purposed delete the pointer will be safe ;P

template <class T> class SharedPointer;
template <class T>
class PointerReference
{
        friend class SharedPointer<T>;
private:
        PointerReference(T* pT) : Count(0), my_pT(pT) { ASSERT(pT != 0); }
        ~PointerReference()         { ASSERT(Count == 0); SafeDelete(my_pT); }

        unsigned GetRef()  { return ++Count; }
        unsigned FreeRef() {  ASSERT(Count!=0); return --Count; }

        T* const my_pT;
        unsigned Count;
};

template <class T>
class SharedPointer
{
public:
        SharedPointer()
        {
            m_Reference = 0;

        }
        SharedPointer(T* pT)
        {
            if(!pT)  { m_Reference = 0; return; }

            m_Reference = new PointerReference<T>(pT);
            m_Reference->GetRef();

        }

        SharedPointer(const SharedPointer<T>& rVar)
        {
                m_Reference = rVar.m_Reference;
                if (!Null())
                        m_Reference->GetRef();
        }

        ~SharedPointer()
        {
                UnBind();
        }

        T& operator*()
        {
            if (Null())
                    return NULL;
                      //  throw NulRefException();
                return *m_Reference->my_pT;
        }
        T* operator->()
        {
                if (Null())
                    return NULL;
                      //  throw NulRefException();
                return m_Reference->my_pT;
        }
        const T* operator->() const
        {
                if (Null())
                    return NULL;
                        //throw NulRefException();
                return m_Reference->my_pT;
        }

        SharedPointer<T>& operator=(const SharedPointer<T>& rVar)
        {
                if (!rVar.Null())
                        rVar.m_Reference->GetRef();
                UnBind();
                m_Reference = rVar.m_Reference;
                return *this;
        }

        bool operator==(const SharedPointer<T>& lhs)
        {
          
            return this->m_Reference == lhs.m_Reference;
        
        }
        
        bool operator!=(const SharedPointer<T>& lhs)
        {
                return !(*this == lhs);
        }

        bool Null() const {return m_Reference == 0; }
        void SetNull() { UnBind(); }

private:
        void UnBind()
        {
                if (!Null() && m_Reference->FreeRef() == 0)       
                        SafeDelete(m_Reference);
                m_Reference = 0;
        }
        PointerReference<T>* m_Reference;
};

#endif
